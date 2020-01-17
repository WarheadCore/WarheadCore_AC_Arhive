
# Add scripts files
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/Transmogrification_SC.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/Transmogrification.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/Transmogrification.h")

# Add scripts to script loader
AC_ADD_SCRIPT_LOADER("Transmogrification" "${CMAKE_CURRENT_LIST_DIR}/src/TransmogrificationLoad.h" "SC_Npc_VisualWeapon")

# Add config file
AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/Transmogrification.conf.dist")
