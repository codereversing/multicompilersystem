#!/bin/bash

run_command () {

    TIMEOUT_SECONDS_COMPILE=15s
    TIMEOUT_SECONDS_RUN=10s

    timeout ${TIMEOUT_SECONDS_COMPILE} g++ -Wall -std=c++17 -Wno-deprecated ${OUTPUT_NAME} -o ${OUTPUT_NAME}.out >> ${OUTPUT_NAME}-output.log 2>&1
    result=$?
    
    if [ $result -eq 0 ]
    then
        chmod 753 ${OUTPUT_NAME}-output.log
        if [ "${ARGS_FILE_EXISTS}" = "true" ]; then
            ARGUMENTS=$(cat ${ARGUMENTS_PATH})
        fi
        if [ "${STDIN_FILE_EXISTS}" = "true" ]; then
            TIMEOUT_SECONDS_RUN=${INTERACTIVE_TIMEOUT}
            STDIN_ARGUMENTS="-s ${STDIN_PATH}"
        fi
        
        sudo su-exec exec ${EXEC_PATH}/executor -t ${TIMEOUT_SECONDS_RUN} ${STDIN_ARGUMENTS} -o ${OUTPUT_NAME}-output.log -f ${OUTPUT_NAME}.out ${ARGUMENTS}
        result=$?
    fi
}
