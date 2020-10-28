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
time sudo apt-get install -y ccache cmake make gcc g++ clang \
  libssl-dev libbz2-dev libreadline-dev libncurses-dev libace-6.* libace-dev \
  libboost-system1.6*-dev libboost-filesystem1.6*-dev libboost-program-options1.6*-dev libboost-iostreams1.6*-dev

case $COMPILER in
  "clang7" )
    time sudo apt-get install -y clang-7
    ;;

  "clang10" )
    time sudo apt-get install -y clang-10
    ;;

  * )
    echo "Unknown compiler $COMPILER"
    exit 1
    ;;
esac

* )
  echo "Unknown CI type ($ci_type)"
  exit 1
    ;;
esac

sudo systemctl start mysql
