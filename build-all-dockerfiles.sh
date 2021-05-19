#!/bin/bash

languages="alpine c cpp cs java py"

for language in $languages; do
    echo "Building image for ${language}"
    sudo docker build -t compiler-base-${language} -f Dockerfiles/${language}/Dockerfile .
    echo "Tagging image for ${language}"
    sudo docker tag compiler-base-${language}:latest localhost:32000/compiler-base-${language}:latest
    echo "Pushing image for ${language}"
    sudo docker push localhost:32000/compiler-base-${language}:latest
done
