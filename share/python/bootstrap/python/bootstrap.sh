#!/bin/bash

run_command () {

    TIMEOUT_SECONDS_RUN=10s
    chmod -R 757 ${WORKSPACE_PATH}/${INDEX}
        
    if [ "${ARGS_FILE_EXISTS}" = "true" ]; then
        ARGUMENTS=$(cat ${ARGUMENTS_PATH})
    fi
    if [ "${STDIN_FILE_EXISTS}" = "true" ]; then
        TIMEOUT_SECONDS_RUN=${INTERACTIVE_TIMEOUT}
        STDIN_ARGUMENTS="-s ${STDIN_PATH}"
    fi
         
    sudo su-exec exec ${EXEC_PATH}/executor -t ${TIMEOUT_SECONDS_RUN} ${STDIN_ARGUMENTS} -o ${OUTPUT_NAME}-output.log -f /usr/bin/python3 ${OUTPUT_NAME} ${ARGUMENTS}
    result=$?
}
