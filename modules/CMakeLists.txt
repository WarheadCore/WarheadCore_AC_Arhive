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

# Группировка сорцов
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_library(modules-interface INTERFACE)

CollectIncludeDirectories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC_INCLUDES)

target_include_directories(modules-interface
  INTERFACE
    ${PUBLIC_INCLUDES})

target_link_libraries(modules-interface
  INTERFACE
    game-interface)

if (NOT DISABLED_ALL_MODULES AND MODULES_EXIST)
# Collect source files not disable modules
CU_SUBDIRLIST(sub_DIRS  "${CMAKE_SOURCE_DIR}/modules" FALSE FALSE)
FOREACH(subdir ${sub_DIRS})
  get_filename_component(MODULENAME ${subdir} NAME)
    
  if (";${DISABLED_WH_MODULES};" MATCHES ";${MODULENAME};")
    continue()
  endif()

  STRING(REGEX REPLACE "^${CMAKE_SOURCE_DIR}/" "" subdir_rel ${subdir}) 
  CollectSourceFiles(
    ${CMAKE_SOURCE_DIR}/${subdir_rel}/
    PRIVATE_SOURCES)

  set(MODULES_STAT_SRCS
    ${MODULES_STAT_SRCS}
    ${PRIVATE_SOURCES})
ENDFOREACH()

add_library(modules STATIC
  ${MODULES_STAT_SRCS})

target_include_directories(modules
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(modules
  PRIVATE
    modules-interface)

add_dependencies(modules revision.h)

set_target_properties(modules
  PROPERTIES
    FOLDER
      "modules")
endif()