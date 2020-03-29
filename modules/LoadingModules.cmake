#
#
#

if (DISABLED_ALL_MODULES)
  message("Disable all modules")
endif()

if (NOT DISABLED_ALL_MODULES)
# Check disabled modules
set(DISABLED_AC_MODULES ${DISABLED_AC_MODULES} ${DISABLED_MODULES})

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
      set(DISABLED_AC_MODULES ${DISABLED_AC_MODULES} ${MODULENAME})
    endif()
  endif()
  
  if (";${DISABLED_AC_MODULES};" MATCHES ";${MODULENAME};")    
    continue()
  endif()

  STRING(REGEX REPLACE "^${CMAKE_SOURCE_DIR}/" "" subdir_rel ${subdir})
  if(EXISTS "${subdir}/CMakeLists.txt")
    message("Loading module: ${subdir_rel}")
    add_subdirectory("${subdir_rel}")
    set(MODULES_EXIST 1)
    AC_ADD_MODULE(${subdir_rel})
  endif()
ENDFOREACH()
endif()