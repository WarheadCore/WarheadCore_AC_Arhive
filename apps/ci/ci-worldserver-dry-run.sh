#!/bin/bash

set -e

(cd ./build/check_install/bin/ && timeout 5m ./worldserver --dry-run)
