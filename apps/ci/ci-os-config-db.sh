#!/bin/bash

set -e

time sudo apt-get update -y
time sudo apt-get install -y git lsb-release sudo ccache
time sudo apt-get install -y libmysqlclient-dev mysql-server curl unzip

cat >>conf/config.sh <<CONFIG_SH
DB_CHARACTERS_CONF="MYSQL_USER='root'; MYSQL_PASS='root'; MYSQL_HOST='localhost';"
DB_AUTH_CONF="MYSQL_USER='root'; MYSQL_PASS='root'; MYSQL_HOST='localhost';"
DB_WORLD_CONF="MYSQL_USER='root'; MYSQL_PASS='root'; MYSQL_HOST='localhost';"
CONFIG_SH

sudo systemctl start mysql
