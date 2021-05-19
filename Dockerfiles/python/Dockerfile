FROM compiler-base-alpine:latest

# Install Python 3.x
RUN apk add --update --no-cache python3 && ln -sf python3 /usr/bin/python
RUN python3 -m ensurepip
RUN pip3 install --no-cache --upgrade pip setuptools

ARG USER=user
ENV CODE_PATH=/home/${USER}/code

# Setup language(s)
ENV LANGUAGE=python
ENV SUPPORTED_LANGUAGES=python
ENV IS_MULTITHREADED=true

USER ${USER}
WORKDIR ${CODE_PATH}

CMD ["./startup.sh"]
