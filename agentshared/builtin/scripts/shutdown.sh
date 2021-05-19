#!/bin/bash

CONTAINER_ID=$(basename $(cat /proc/1/cpuset))

kill_agent () {
    killall -9 agent
}

relocate_input () {
    mkdir ${CODE_PATH}/share/relocate/input/${CONTAINER_ID}
    mkdir ${CODE_PATH}/share/relocate/arguments/${CONTAINER_ID}
    mv ${CODE_PATH}/share/${LANGUAGE}/input/${CONTAINER_ID}/* ${CODE_PATH}/share/relocate/input/${CONTAINER_ID}/
    mv ${CODE_PATH}/share/${LANGUAGE}/arguments/${CONTAINER_ID}/* ${CODE_PATH}/share/relocate/arguments/${CONTAINER_ID}/
}

delete_folders () {
    rm -rf ${CODE_PATH}/share/${LANGUAGE}/input/${CONTAINER_ID}
    rm -rf ${CODE_PATH}/share/${LANGUAGE}/output/${CONTAINER_ID}
    rm -rf ${CODE_PATH}/share/${LANGUAGE}/workspace/${CONTAINER_ID}
    rm -rf ${CODE_PATH}/share/${LANGUAGE}/arguments/${CONTAINER_ID}
    rm -rf ${CODE_PATH}/share/${LANGUAGE}/stdin/${CONTAINER_ID}
}

main () {
    relocate_input
    delete_folders
    kill_agent
}

main
