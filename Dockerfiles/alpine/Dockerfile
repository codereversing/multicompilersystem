FROM n0madic/alpine-gcc:9.2.0

RUN apk add --update --no-cache su-exec inotify-tools build-base busybox-suid sudo

# Setup user
ARG USER=user
ENV HOME=/home/${USER}
ENV EXEC=exec
ENV EXEC_HOME=/home/${EXEC}

ENV CODE_PATH=/home/${USER}/code
ENV EXEC_PATH=/home/${EXEC}/code

RUN mkdir ${HOME}
RUN mkdir ${CODE_PATH}

RUN mkdir ${EXEC_HOME}
RUN mkdir ${EXEC_PATH}

ADD agentshared ${CODE_PATH}

# Build file watcher code
RUN g++ -std=c++17 -o ${CODE_PATH}/agent -I ${CODE_PATH}/builtin/code/agent/thirdparty/cereal/include \
    -I ${CODE_PATH}/builtin/code/agent/thirdparty/thread_pools/include \ 
    ${CODE_PATH}/builtin/code/agent/src/*.cpp \ 
    ${CODE_PATH}/builtin/code/agent/src/Agent/Notify/*.cpp \ 
    -lstdc++fs -pthread

# Build executor code
RUN g++ -std=c++17 -o ${EXEC_PATH}/executor ${CODE_PATH}/builtin/code/executor/src/Source.cpp -lstdc++fs -pthread

RUN mv ${CODE_PATH}/builtin/scripts/startup.sh ${CODE_PATH}
RUN mv ${CODE_PATH}/builtin/scripts/shutdown.sh ${CODE_PATH}
RUN mv ${CODE_PATH}/builtin/config/config.json ${CODE_PATH}
RUN rm -rf ${CODE_PATH}/builtin

RUN adduser -D ${USER} && echo "$USER ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/$USER && chmod 0440 /etc/sudoers.d/$USER
RUN adduser -D exec

RUN sudo passwd -d root
RUN sudo passwd -d ${USER}

RUN echo 'user ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers

RUN chown -R ${USER}:${USER} ${HOME}
RUN chmod -R 751 ${HOME}

RUN chown -R ${EXEC}:${EXEC} ${EXEC_HOME}
RUN chmod -R 555 ${EXEC_PATH}
