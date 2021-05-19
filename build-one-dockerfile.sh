#!/bin/bash

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -l|--language)
    LANGUAGE="$2"
    shift
    shift
    ;;
    *)
    POSITIONAL+=("$1")
    shift
    ;;
esac
done
set -- "${POSITIONAL[@]}"

echo "Building image for ${LANGUAGE}"
sudo docker build -t compiler-base-${LANGUAGE} -f Dockerfiles/${LANGUAGE}/Dockerfile .
echo "Tagging image for ${LANGUAGE}"
sudo docker tag compiler-base-${LANGUAGE}:latest localhost:32000/compiler-base-${LANGUAGE}:latest
echo "Pushing image for ${LANGUAGE}"
sudo docker push localhost:32000/compiler-base-${LANGUAGE}:latest
