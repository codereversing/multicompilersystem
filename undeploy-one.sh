#!/bin/bash

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -l|--language)
    LANGUAGE="$2"
    shift # past argument
    shift # past value
    ;;
    *)    # unknown option
    POSITIONAL+=("$1") # save it in an array for later
    shift # past argument
    ;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

echo "Deleting deployment for ${LANGUAGE}"
sudo microk8s.kubectl delete -f Deployments/${LANGUAGE}/deployment.yaml
echo "Deleted deployment for ${LANGUAGE}"
