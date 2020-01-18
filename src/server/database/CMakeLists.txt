#
# Copyright (C) 2016+     AzerothCore <www.azerothcore.org>
#

CollectSourceFiles(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE_SOURCES)

# Group sources
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DACORE_API_EXPORT_DATABASE)

add_library(database
  ${PRIVATE_SOURCES})

CollectIncludeDirectories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC_INCLUDES)

target_include_directories(database
  PUBLIC
    ${PUBLIC_INCLUDES}
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(database
  PRIVATE
    acore-core-interface
  PUBLIC
    common)

CU_GET_GLOBAL("AC_MODULE_LIST")
FOREACH(moduleList ${AC_MODULE_LIST})
    get_filename_component(file_name ${moduleList} NAME_WE)
    set(MODULE_LIST__ ${MODULE_LIST__}${file_name},)
ENDFOREACH()

add_definitions(-DAC_MODULES_LIST=$<1:"${MODULE_LIST__}">)

set_target_properties(database
  PROPERTIES
    FOLDER
      "server")

if( BUILD_SHARED_LIBS )
  if( UNIX )
    install(TARGETS database
      LIBRARY
        DESTINATION lib)
  elseif( WIN32 )
    install(TARGETS database
      RUNTIME
        DESTINATION "${CMAKE_INSTALL_PREFIX}")
  endif()
endif()