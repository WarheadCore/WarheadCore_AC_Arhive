#!/bin/bash

set -e

echo "--> Start worldserver dry-run"
git clone --depth=1 --branch=master --single-branch https://github.com/ac-data/ac-data.git /home/travis/build/$TRAVIS_REPO_SLUG/env/dist/data
sed -e "s/;;acore_auth/;$DB_RND_NAME;auth_$DB_RND_NAME/" -e "s/;;acore_world/;$DB_RND_NAME;world_$DB_RND_NAME/" -e "s/;;acore_characters/;$DB_RND_NAME;characters_$DB_RND_NAME/" ./data/travis/worldserver.conf >./env/dist/etc/worldserver.conf
./env/dist/bin/worldserver --dry-run
