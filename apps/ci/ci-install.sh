#!/bin/bash

set -e

ci_type=$1

time sudo apt-get update -y
time sudo apt-get install -y git lsb-release sudo ccache
time sudo apt-get install -y libmysqlclient-dev mysql-server curl unzip

case $ci_type in
"import-db" )
cat >>conf/config.sh <<CONFIG_SH
DB_CHARACTERS_CONF="MYSQL_USER='root'; MYSQL_PASS='root'; MYSQL_HOST='localhost';"
DB_AUTH_CONF="MYSQL_USER='root'; MYSQL_PASS='root'; MYSQL_HOST='localhost';"
DB_WORLD_CONF="MYSQL_USER='root'; MYSQL_PASS='root'; MYSQL_HOST='localhost';"
CONFIG_SH
  ;;

"core-build" )
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

time sudo apt-get install -y ccache cmake make gcc g++ clang \
  libssl-dev libbz2-dev libreadline-dev libncurses-dev libace-6.* libace-dev \
  libboost-system1.65-dev libboost-filesystem1.65-dev libboost-thread1.65-dev libboost-program-options1.65-dev libboost-iostreams1.65-dev

time sudo apt-get install -y clang-7
  echo "CCOMPILERC=\"clang-7\"" >> ./conf/config.sh
  echo "CCOMPILERCXX=\"clang++-7\"" >> ./conf/config.sh
  ;;

* )
  echo "Unknown CI type ($ci_type)"
  exit 1
    ;;
esac

sudo systemctl start mysql
