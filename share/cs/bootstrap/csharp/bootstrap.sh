#!/bin/bash

run_command () {

    TIMEOUT_SECONDS_RUN=15s
    chmod -R 757 ${WORKSPACE_PATH}/${INDEX}
    
    if [ "${ARGS_FILE_EXISTS}" = "true" ]; then
        ARGUMENTS=$(cat ${ARGUMENTS_PATH})
    fi
    if [ "${STDIN_FILE_EXISTS}" = "true" ]; then
        TIMEOUT_SECONDS_RUN=${INTERACTIVE_TIMEOUT}
        STDIN_ARGUMENTS="-s ${STDIN_PATH}"
    fi
        
    sudo su-exec exec ${EXEC_PATH}/executor -t ${TIMEOUT_SECONDS_RUN} ${STDIN_ARGUMENTS} -o ${OUTPUT_NAME}-output.log -f /usr/dotnet/dotnet run --project ${WORKSPACE_PATH}/${INDEX} -- ${ARGUMENTS}
    result=$?
}

