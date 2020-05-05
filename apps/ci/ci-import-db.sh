#!/bin/bash

set -e

sudo systemctl start mysql
./acore.sh "db-assembler" "import-all"
