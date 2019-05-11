#!/bin/bash

chmod +x contrib/check_updates.sh

# Create DB
mysql -uroot -e 'create database test_mysql;'
mysql -uroot < sql/create/create_mysql.sql

# Update Auth
mysql -uwarhead -pwarhead auth < sql/base/auth_database.sql
./contrib/check_updates.sh auth 3.3.5 auth localhost

# Update Character
mysql -uwarhead -pwarhead characters < sql/base/characters_database.sql
./contrib/check_updates.sh characters 3.3.5 characters localhost

# Update World
mysql -uwarhead -pwarhead world < sql/base/dev/world_database.sql
cat sql/updates/world/3.3.5/*.sql | mysql -uwarhead -pwarhead world

# Drop DB
mysql -uroot < sql/create/drop_mysql.sql