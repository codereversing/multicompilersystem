#!/bin/bash

CONTAINER_ID=$(basename $(cat /proc/1/cpuset))

create_folders () {
    mkdir ${CODE_PATH}/share/${LANGUAGE}/input/${CONTAINER_ID}
    mkdir ${CODE_PATH}/share/${LANGUAGE}/output/${CONTAINER_ID}
    mkdir ${CODE_PATH}/share/${LANGUAGE}/workspace/${CONTAINER_ID}
    mkdir ${CODE_PATH}/share/${LANGUAGE}/arguments/${CONTAINER_ID}
    mkdir ${CODE_PATH}/share/${LANGUAGE}/stdin/${CONTAINER_ID}
}

fix_config () {
    sed -i "s|\${CODE_PATH}|${CODE_PATH}|g" ${CODE_PATH}/config.json
    sed -i "s|\${UNIQUE_ID}|${CONTAINER_ID}|g" ${CODE_PATH}/config.json
    sed -i "s|\${LANGUAGE}|${LANGUAGE}|g" ${CODE_PATH}/config.json
    sed -i "s|\${SUPPORTED_LANGUAGES}|\"${SUPPORTED_LANGUAGES}\"|g" ${CODE_PATH}/config.json
    sed -i "s|\${IS_MULTITHREADED}|${IS_MULTITHREADED}|g" ${CODE_PATH}/config.json
}

start_agent () {
    ./agent config.json
}

launch_dotnet () {
    dotnet run
    sudo su-exec exec dotnet run
}

main () {
    launch_dotnet
    create_folders
    fix_config
    start_agent
}

main
