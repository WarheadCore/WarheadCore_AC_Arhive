#
# This file is part of the WarheadCore Project. See AUTHORS file for Copyright information
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# User has manually chosen to ignore the git-tests, so throw them a warning.
# This is done EACH compile so they can be alerted about the consequences.
#

# Add scripts files
WH_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/Transmogrification_SC.cpp")
WH_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/Transmogrification.cpp")
WH_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/Transmogrification.h")

# Add scripts to script loader
WH_ADD_SCRIPT_LOADER("Transmogrification" "${CMAKE_CURRENT_LIST_DIR}/src/TransmogrificationLoad.h" "SC_Npc_VisualWeapon")

# Add config file
WH_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/Transmogrification.conf.dist")
