#!/bin/bash

languages="c cpp csharp java python"

for language in $languages; do
    echo "Deleting deployment for ${language}"
    sudo microk8s.kubectl delete -f Deployments/${language}/deployment.yaml
    echo "Deleted deployment for ${language}"
done
