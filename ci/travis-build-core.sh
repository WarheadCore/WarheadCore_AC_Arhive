#!/bin/bash

# Build
cd bin
make -j 4 -k && make install

# Check versions
./check_install/bin/authserver --version
./check_install/bin/worldserver --version

# Go to build dir
cd $TRAVIS_BUILD_DIR