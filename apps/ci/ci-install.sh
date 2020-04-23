#!/bin/bash

set -e

echo "--> Install OS deps (apt-get)"
bash ./acore.sh "install-deps"

echo "--> Install clang-7"
sudo apt-get install clang-7

echo "--> Create config.sh"
cat >>conf/config.sh <<CONFIG_SH
MTHREADS=$(expr $(grep -c ^processor /proc/cpuinfo) + 2)
CWARNINGS=ON
CDEBUG=OFF
CTYPE=Release
CSCRIPTS=ON
CSERVERS=ON
CTOOLS=ON
CSCRIPTPCH=ON
CCOREPCH=ON
CCUSTOMOPTIONS='-DCMAKE_C_FLAGS="-Werror" -DCMAKE_CXX_FLAGS="-Werror"'
DB_CHARACTERS_CONF="MYSQL_USER='root'; MYSQL_PASS='$DB_RND_NAME'; MYSQL_HOST='localhost';"
DB_AUTH_CONF="MYSQL_USER='root'; MYSQL_PASS='$DB_RND_NAME'; MYSQL_HOST='localhost';"
DB_WORLD_CONF="MYSQL_USER='root'; MYSQL_PASS='$DB_RND_NAME'; MYSQL_HOST='localhost';"
DB_AUTH_NAME=auth_$DB_RND_NAME
DB_CHARACTERS_NAME=characters_$DB_RND_NAME
DB_WORLD_NAME=world_$DB_RND_NAME
CONFIG_SH

time sudo apt-get update -y
# time sudo apt-get upgrade -y
time sudo apt-get install -y git lsb-release sudo ccache
time ./acore.sh install-deps

case $COMPILER in
  "clang6" )
    time sudo apt-get install -y clang-6.0
    echo "CCOMPILERC=\"clang-6.0\"" >> ./conf/config.sh
    echo "CCOMPILERCXX=\"clang++-6.0\"" >> ./conf/config.sh
    ;;

  "clang7" )
    time sudo apt-get install -y clang-7
    echo "CCOMPILERC=\"clang-7\"" >> ./conf/config.sh
    echo "CCOMPILERCXX=\"clang++-7\"" >> ./conf/config.sh
    ;;

  * )
    echo "Unknown compiler $COMPILER"
    exit 1
    ;;
esac
