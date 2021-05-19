FROM compiler-base-alpine:latest

ARG USER=user
ENV CODE_PATH=/home/${USER}/code

# Setup language(s)
ENV LANGUAGE=c
ENV SUPPORTED_LANGUAGES=c
ENV IS_MULTITHREADED=true

USER ${USER}
WORKDIR ${CODE_PATH}

CMD ["./startup.sh"]
