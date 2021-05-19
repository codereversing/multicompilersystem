#include <algorithm>
#include <array>
#include <cstdio>
#include <future>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <limits.h>
#include <pty.h>
#include <signal.h>
#include <string>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

typedef struct
{
    bool IsInteractive;
    int MaxWaitTimeMs;
    std::string BinaryToExecute;
    std::string OutputFilePath;
    std::string InputFilePath;
    std::vector<char*> BinaryArguments;
} CommandLineOptions;

std::fstream g_outputFile;

void ListenForOutput(const int masterFileDescriptor, const std::string outputFilePath)
{
    g_outputFile.rdbuf()->pubsetbuf(0, 0);
    g_outputFile.open(outputFilePath, std::ios::out);

    constexpr int BUFFERSIZE = 1024;
    std::array<char, BUFFERSIZE> buffer;
    fd_set fileDescriptors = { 0 };

    while (true)
    {
        FD_ZERO(&fileDescriptors);
        FD_SET(masterFileDescriptor, &fileDescriptors);

        if (select(masterFileDescriptor + 1, &fileDescriptors, NULL, NULL, NULL) > 0)
        {
            auto bytesRead = read(masterFileDescriptor, buffer.data(), buffer.size());
            if (bytesRead > 0)
            {
                g_outputFile.write(buffer.data(), bytesRead);
            }
        }
    }
}

void ListenForInput(const int masterFileDescriptor, const std::string inputFileFullPath)
{
    int notifyFileDescriptor = inotify_init();
    if (notifyFileDescriptor == -1)
    {
        perror("inotify_init");
        exit(-1);
    }

    std::string inputDirectory = std::filesystem::absolute(inputFileFullPath).parent_path();
    int watchFileDescriptor = inotify_add_watch(notifyFileDescriptor, inputDirectory.c_str(), IN_CLOSE_WRITE);
    if (watchFileDescriptor == -1)
    {
        perror("inotify_add_watch");
        exit(-1);
    }

    constexpr auto BUFFER_SIZE = (10 * (sizeof(struct inotify_event) + NAME_MAX + 1));
    std::array<char, BUFFER_SIZE> readBuffer;
    std::streamoff lastReadOffset = 0;
    std::string stdinFileName = std::filesystem::path(inputFileFullPath).filename();

    while (true)
    {
        ssize_t bytesRead = read(notifyFileDescriptor, readBuffer.data(), readBuffer.size());
        if (bytesRead == -1)
        {
            perror("read");
            break;
        }

        for (auto* bufferStart = readBuffer.data(); bufferStart < readBuffer.data() + bytesRead; /*Empty*/)
        {
            inotify_event* pEvent = (inotify_event*)bufferStart;

            if (pEvent->len > 0)
            {
                if (pEvent->mask & IN_CLOSE_WRITE)
                {
                    std::string fileName = pEvent->name;

                    if (fileName == stdinFileName)
                    {
                        std::ifstream file(inputFileFullPath, std::ios::in | std::ios::binary);

                        std::vector<char> contents;
                        file.seekg(lastReadOffset, std::ios::end);
                        contents.reserve(file.tellg());
                        file.seekg(lastReadOffset, std::ios::beg);

                        contents.assign((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

                        lastReadOffset += contents.size();

                        size_t bytesWritten = 0;
                        do
                        {
                            ssize_t written = write(masterFileDescriptor, contents.data() + bytesWritten, contents.size() - bytesWritten);
                            if (written == -1)
                            {
                                perror("write");
                                break;
                            }
                            bytesWritten += written;
                        } while (bytesWritten < contents.size());
                    }
                }
            }

            bufferStart += sizeof(inotify_event) + pEvent->len;
        }
    }
}

pid_t WaitForCloseOrTimeout(const pid_t childProcessId, const int maxWaitTimeMs)
{
    int childReturnCode = -1;

    constexpr int sleepTimeMicroseconds = 100000;
    int elapsedTimeMs = 0;
    bool timeoutExpired = false;
    bool childExited = false;
    while (!timeoutExpired && !childExited)
    {
        int result = waitpid(childProcessId, &childReturnCode, WNOHANG);
        if (result == -1)
        {
            perror("waitpid");
        }
        if (result == childProcessId)
        {
            childExited = true;
        }

        usleep(sleepTimeMicroseconds);
        elapsedTimeMs += sleepTimeMicroseconds / 1000;
        timeoutExpired = (elapsedTimeMs >= maxWaitTimeMs);
    }

    if (timeoutExpired)
    {
        constexpr int timeoutReturnCode = 124;
        childReturnCode = timeoutReturnCode;
        kill(-childProcessId, SIGTERM);
    }

    return childReturnCode;
}

int GetWaitTimeMs(const std::string argument)
{
    int waitTimeMs = -1;

    char lastCharacter = argument[argument.length() - 1];
    switch (lastCharacter)
    {
    case 's':
        waitTimeMs = std::stoi(argument) * 1000;
        break;
    case 'm':
        waitTimeMs = std::stoi(argument) * 60 * 1000;
        break;
    case 'h':
        waitTimeMs = std::stoi(argument) * 60 * 60 * 1000;
        break;
    }

    return waitTimeMs;
}

CommandLineOptions GetCommandLineOptions(int argc, char *argv[])
{
    CommandLineOptions options = { 0 };

    int c = 0;
    bool continueParsing = true;
    while (continueParsing && (c = getopt(argc, (char**)argv, "t:f:a:o:s:")) != -1)
    {
        switch ((char)c)
        {
        case 't':
            options.MaxWaitTimeMs = GetWaitTimeMs(optarg);
            break;
        case 'f':
            options.BinaryToExecute = optarg;
            break;
        case 'o':
            options.OutputFilePath = optarg;
            break;
        case '?':
            optind--;
            continueParsing = false;
            break;
        case 's':
            options.IsInteractive = true;
            options.InputFilePath = optarg;
            break;
        }
    }

    options.BinaryArguments.push_back(const_cast<char *>(options.BinaryToExecute.c_str()));
    while (optind < argc)
    {
        char* binaryArgument = argv[optind];
        options.BinaryArguments.push_back(binaryArgument);
        optind++;
    }
    options.BinaryArguments.push_back(nullptr);

    return options;
}

int main(int argc, char* argv[])
{
    CommandLineOptions options = GetCommandLineOptions(argc, argv);

    if (options.MaxWaitTimeMs == -1 || options.BinaryToExecute.empty())
    {
        std::cerr << "Invalid arguments." << std::endl;
        exit(-1);
    }

    int masterFileDescriptor = -1;
    pid_t childProcessId = forkpty(&masterFileDescriptor, NULL, NULL, NULL);
    int childReturnCode = -1;

    if (childProcessId == -1)
    {
        perror("forkpty");
        exit(-1);
    }

    if (childProcessId == 0)
    {
        int result = execv(options.BinaryToExecute.c_str(), options.BinaryArguments.data());
        exit(result);
    }
    else
    {
        std::thread(ListenForOutput, masterFileDescriptor, options.OutputFilePath)
            .detach();

        if (options.IsInteractive)
        {
            std::thread(ListenForInput, masterFileDescriptor, options.InputFilePath)
                .detach();
        }

        childReturnCode = WaitForCloseOrTimeout(childProcessId, options.MaxWaitTimeMs);
    }

    return childReturnCode;
}