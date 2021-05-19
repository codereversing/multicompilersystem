#!/bin/bash

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -f|--filepath)
    FILE_PATH="$2"
    shift
    shift
    ;;
    -a|--arguments)
    ARGUMENTS_PATH="$2"
    shift
    shift
    ;;
    -i|--index)
    INDEX="$2"
    shift
    shift
    ;;
    -d|--dependenciespath)
    DEPENDENCIES_PATH="$2"
    shift
    shift
    ;;
    -w|--workspacepath)
    WORKSPACE_PATH="$2"
    shift
    shift
    ;;
    -o|--outputpath)
    OUTPUT_PATH="$2"
    shift
    shift
    ;;
    -s|--stdinPath)
    STDIN_PATH="$2"
    shift
    shift
    ;;
    -t|--timeout)
    INTERACTIVE_TIMEOUT="$2"
    shift
    shift
    ;;
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

FILE_NAME="$(basename ${FILE_PATH})"
ARGS_FILE_EXISTS=false
STDIN_FILE_EXISTS=false

OUTPUT_NAME=${WORKSPACE_PATH}/${INDEX}/${FILE_NAME}

if [ -f "${ARGUMENTS_PATH}" ]; then
    ARGS_FILE_EXISTS=true
fi

if [ -f "${STDIN_PATH}" ]; then
    STDIN_FILE_EXISTS=true
fi

create_directories () {
    rm -rf ${WORKSPACE_PATH}/${INDEX}
    mkdir ${WORKSPACE_PATH}/${INDEX}
}

cleanup () {
    rm ${FILE_PATH}
    if [ "${ARGS_FILE_EXISTS}" = "true" ]; then
        rm ${ARGUMENTS_PATH}
    fi
    if [ "${STDIN_FILE_EXISTS}" = "true" ]; then
        rm ${STDIN_PATH}
    fi
    rm -rf ${WORKSPACE_PATH}/${INDEX}
}

copy_dependencies () {
    cp ${FILE_PATH} ${WORKSPACE_PATH}/${INDEX}
    cp -r ${DEPENDENCIES_PATH}/. ${WORKSPACE_PATH}/${INDEX}
}

move_output () {
    rm -rf ${OUTPUT_PATH}/${INDEX}
    mkdir ${OUTPUT_PATH}/${INDEX}
    mv ${OUTPUT_NAME}-output.log ${OUTPUT_PATH}/${INDEX}
}

main () {
    create_directories
    copy_dependencies
    (cd ${CODE_PATH}/share/${LANGUAGE}/bootstrap/${LANGUAGE}; source ./bootstrap.sh; run_command)
    move_output
    cleanup
    
    exit ${result}
}

main
