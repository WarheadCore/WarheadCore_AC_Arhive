#!/bin/bash

set -e

(cd ./env/dist/bin/ && timeout 5m ./worldserver --dry-run)
