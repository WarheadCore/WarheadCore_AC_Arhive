#!/bin/bash

set -e

echo "--> Import DB"
bash ./acore.sh "db-assembler" "import-all"
