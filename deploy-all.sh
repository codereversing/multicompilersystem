#!/bin/bash

LANGUAGES="c cpp cs java py"

for LANGUAGE in $LANGUAGES; do
    CURRENT_USER=${USER}
    REPLACE_USER=`cat "Deployments/${LANGUAGE}/deployment.yaml" | sed "s/{{USER}}/$CURRENT_USER/g"`
    echo "Deploying containers for ${LANGUAGE}"
    echo "$REPLACE_USER" > Deployments/${LANGUAGE}/deployment.runtime.yaml
    sudo microk8s.kubectl apply -f Deployments/${LANGUAGE}/deployment.runtime.yaml
    echo "Deployed containers for ${LANGUAGE}"
done
