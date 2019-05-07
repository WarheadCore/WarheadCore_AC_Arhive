#!/bin/bash

set -e

echo "Send status to Dicord"

wget https://raw.githubusercontent.com/WarheadCore/travis-ci-discord-webhook/master/send.sh
chmod +x send.sh
bash ./send.sh $1 $2
