FROM compiler-base-alpine:latest

# Install .NET Core
RUN apk add --update --no-cache bash wget icu-libs openssl
RUN wget https://dot.net/v1/dotnet-install.sh && chmod +x dotnet-install.sh && ./dotnet-install.sh -c Current -InstallDir /usr/dotnet/

ARG USER=user
ENV CODE_PATH=/home/${USER}/code

# Setup language(s)
ENV LANGUAGE=cs
ENV SUPPORTED_LANGUAGES=cs
ENV IS_MULTITHREADED=true

# Setup PATH
ENV PATH=/usr/dotnet:${PATH}

USER ${USER}
WORKDIR ${CODE_PATH}

CMD ["./startup.sh"]
