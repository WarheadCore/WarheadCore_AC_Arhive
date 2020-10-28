#!/bin/bash

set -e

echo "[worldserver]" >> ./build/check_install/etc/worldserver.conf
echo "DataDir = \"../data/\"" >> ./build/check_install/etc/worldserver.conf
echo "LoginDatabaseInfo     = \"localhost;3306;root;root;acore_auth\"" >> ./build/check_install/etc/worldserver.conf
echo "WorldDatabaseInfo     = \"localhost;3306;root;root;acore_world\"" >> ./build/check_install/etc/worldserver.conf
echo "CharacterDatabaseInfo = \"localhost;3306;root;root;acore_characters\"" >> ./build/check_install/etc/worldserver.conf
git clone --depth=1 --branch=master --single-branch https://github.com/ac-data/ac-data.git ./build/check_install/data
(cd ./build/check_install/bin/ && timeout 5m ./worldserver --import-db)
