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

CURRENT_USER=${USER}
REPLACE_USER=`cat "Deployments/${LANGUAGE}/deployment.yaml" | sed "s/{{USER}}/$CURRENT_USER/g"`
echo "Deploying containers for ${LANGUAGE}"
echo "$REPLACE_USER" > Deployments/${LANGUAGE}/deployment.runtime.yaml
sudo microk8s.kubectl apply -f Deployments/${LANGUAGE}/deployment.runtime.yaml
echo "Deployed containers for ${LANGUAGE}"
