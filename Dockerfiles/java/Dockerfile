FROM compiler-base-alpine:latest

# Install Java
RUN apk add --update --no-cache openjdk11 --repository=http://dl-cdn.alpinelinux.org/alpine/edge/community

# Setup Java environment
ENV JAVA_HOME=/usr/lib/jvm/java-11-openjdk
RUN export JAVA_HOME

ARG USER=user
ENV CODE_PATH=/home/${USER}/code

# Setup language(s)
ENV LANGUAGE=java
ENV SUPPORTED_LANGUAGES=java
ENV IS_MULTITHREADED=true

# Setup PATH
ENV PATH=/usr/lib/jvm/java-11-openjdk/bin:${PATH}

USER ${USER}
WORKDIR ${CODE_PATH}

CMD ["./startup.sh"]
