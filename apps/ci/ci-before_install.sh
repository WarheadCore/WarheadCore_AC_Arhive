#!/bin/bash

set -e

echo "--> Set root password"
export DB_RND_NAME=$(cat /dev/urandom | tr -dc 'a-z' | fold -w 5 | head -n 1)
sudo mysql -e "use mysql; update user set authentication_string=PASSWORD('$DB_RND_NAME') where User='root'; update user set plugin='mysql_native_password';FLUSH PRIVILEGES;"
sudo mysql_upgrade -u root -p$DB_RND_NAME
sudo service mysql restart
printf "[client]\npassword=%s" "$DB_RND_NAME" >~/.my.cnf
chmod 400 ~/.my.cnf
