#!/bin/bash

set -e

cat >>conf/config.sh <<CONFIG_SH
MTHREADS=$(expr $(grep -c ^processor /proc/cpuinfo) + 2)
CWARNINGS=OFF
CDEBUG=OFF
CTYPE=Release
CSCRIPTS=ON
CSERVERS=ON
CTOOLS=ON
CSCRIPTPCH=OFF
CCOREPCH=OFF
CCUSTOMOPTIONS='-DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache'
DB_CHARACTERS_CONF="MYSQL_USER='root'; MYSQL_PASS='root'; MYSQL_HOST='localhost';"
DB_AUTH_CONF="MYSQL_USER='root'; MYSQL_PASS='root'; MYSQL_HOST='localhost';"
DB_WORLD_CONF="MYSQL_USER='root'; MYSQL_PASS='root'; MYSQL_HOST='localhost';"
CONFIG_SH

time sudo apt-get update -y
# time sudo apt-get upgrade -y
time sudo apt-get install -y git lsb-release sudo ccache
time sudo apt-get install -y git cmake make gcc g++ clang libmysqlclient-dev \
  libssl-dev libbz2-dev libreadline-dev libncurses-dev \
  mysql-server libace-6.* libace-dev curl unzip \
  libboost-system1.65-dev libboost-filesystem1.65-dev libboost-thread1.65-dev libboost-program_options1.65-dev libboost-iostreams1.65-dev

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
