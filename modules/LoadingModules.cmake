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

if (DISABLED_ALL_MODULES)
  message("Disable all modules")
endif()

if (NOT DISABLED_ALL_MODULES)
# Check disabled modules
set(DISABLED_WH_MODULES ${DISABLED_WH_MODULES} ${DISABLED_MODULES})

# Set empty modules default
set(MODULES_EXIST 0)

# Check dirs for disable
CU_SUBDIRLIST(sub_DIRS  "${CMAKE_SOURCE_DIR}/modules" FALSE FALSE)
FOREACH(subdir ${sub_DIRS})
  get_filename_component(MODULENAME ${subdir} NAME)

  # Display disabled modules in Windows
  if(WIN32 AND DISPLAY_CMAKE_GUI_OPTION AND NOT ";${DISABLED_MODULES};" MATCHES ";${MODULENAME};")  
    option(MODULE_${MODULENAME} "Enable module:${MODULENAME}" 1)

    if (NOT MODULE_${MODULENAME})
      set(DISABLED_WH_MODULES ${DISABLED_WH_MODULES} ${MODULENAME})
    endif()
  endif()
  
  if (";${DISABLED_WH_MODULES};" MATCHES ";${MODULENAME};")    
    continue()
  endif()

  STRING(REGEX REPLACE "^${CMAKE_SOURCE_DIR}/" "" subdir_rel ${subdir})
  if(EXISTS "${subdir}/CMakeLists.txt")
    message("Loading module: ${subdir_rel}")
    add_subdirectory("${subdir_rel}")
    set(MODULES_EXIST 1)
    WH_ADD_MODULE(${subdir_rel})
  endif()
ENDFOREACH()
endif()