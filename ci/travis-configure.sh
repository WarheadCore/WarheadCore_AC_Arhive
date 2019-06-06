#!/bin/bash

set -e

mkdir bin
cd bin
cmake ../ -DWITH_WARNINGS=1 -DSCRIPTS=dynamic -DNOJEM=0 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=check_install
cd ..