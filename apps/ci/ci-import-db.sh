#!/bin/bash

set -e

sudo systemctl start mysql
./apps/db_assembler/db_assembler.sh "import-all"
