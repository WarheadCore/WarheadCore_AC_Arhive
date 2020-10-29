#!/bin/bash

set -e

MTHREADS="$(($(grep -c ^processor /proc/cpuinfo) + 2))"

echo "Compile core"
export CCACHE_CPP2=true
export CCACHE_MAXSIZE='500MB'
export CCACHE_COMPRESS=1
ccache -s
cd build
echo "> Using $MTHREADS threads"
make -j "$MTHREADS" -k && make -j "$MTHREADS" install
cd check_install/bin
ccache -s
