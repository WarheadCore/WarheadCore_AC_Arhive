#!/bin/bash

set -e

echo "--> Compile core"
timeout 2700 bash ./acore.sh "compiler" "all"
