# Copyright (C)
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# Enable precompiled headers when using the GCC compiler.

CU_RUN_HOOK(BEFORE_SCRIPTS_LIBRARY)

CU_SET_PATH("AC_SCRIPTS_DIR" "${CMAKE_CURRENT_LIST_DIR}")

message(STATUS "SCRIPT PREPARATIONS")

set("AC_DEF_SCRIPTS_LIST" "")

macro(AC_ADD_DEF_SCRIPT_LOADER script_dec)
  set("AC_DEF_SCRIPTS_LIST" "void Add${script_dec}Scripts();${AC_DEF_SCRIPTS_LIST}")
endmacro()

macro(PrepareScripts name out)
  file(GLOB_RECURSE found
    ${name}/*.h
    ${name}/*.cpp)
  
  list(APPEND ${out} ${found})
  AC_ADD_DEF_SCRIPT_LOADER("${name}")
  AC_ADD_SCRIPT_LOADER("${name}" "")
  message(STATUS "  -> Prepared: ${name}")
endmacro(PrepareScripts)

CU_GET_GLOBAL("AC_SCRIPTS_SOURCES")

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ScriptLoader.h
  ${BUILDDIR}/GenLoader.cpp
  ${AC_SCRIPTS_SOURCES})

CU_SET_PATH("CMAKE_AC_MODULE_DIR" "${CMAKE_CURRENT_LIST_DIR}")

PrepareScripts(Spells scripts_STAT_SRCS)
PrepareScripts(Commands scripts_STAT_SRCS)

if(SCRIPTS)
  PrepareScripts(Custom scripts_STAT_SRCS)
  PrepareScripts(World scripts_STAT_SRCS)
  PrepareScripts(OutdoorPvP scripts_STAT_SRCS)
  PrepareScripts(EasternKingdoms scripts_STAT_SRCS)
  PrepareScripts(Kalimdor scripts_STAT_SRCS)
  PrepareScripts(Outland scripts_STAT_SRCS)
  PrepareScripts(Northrend scripts_STAT_SRCS)
  PrepareScripts(Events scripts_STAT_SRCS)
  PrepareScripts(Pet scripts_STAT_SRCS)
endif()

CU_GET_GLOBAL("AC_ADD_SCRIPTS_LIST")
CU_GET_GLOBAL("AC_ADD_SCRIPTS_INCLUDE")

set("AC_SCRIPTS_INCLUDES" "")
set("AC_MODULE_LIST" "")
set("AC_SCRIPTS_LIST" "")

FOREACH (include ${AC_ADD_SCRIPTS_INCLUDE})
  set("AC_SCRIPTS_INCLUDES" "#include \"${include}\"\n${AC_SCRIPTS_INCLUDES}")
ENDFOREACH()

FOREACH (void ${AC_ADD_SCRIPTS_LIST})
  set("AC_MODULE_LIST" "void ${void};\n${AC_MODULE_LIST}")
ENDFOREACH()

FOREACH (scpript_list ${AC_DEF_SCRIPTS_LIST})
  set("AC_SCRIPTS_LIST" "${scpript_list};\n${AC_SCRIPTS_LIST}")
ENDFOREACH()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/GenLoader.cpp.cmake ${BUILDDIR}/GenLoader.cpp)

message(STATUS "SCRIPT PREPARATION COMPLETE")
message("")

if (USE_SCRIPTPCH)
  set(PRIVATE_PCH_HEADER PrecompiledHeaders/ScriptPCH.h)
endif ()

# Group sources
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_library(scripts-interface INTERFACE)

CollectIncludeDirectories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC_INCLUDES
  # Exclude
  ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders)

target_include_directories(scripts-interface
  INTERFACE
    ${PUBLIC_INCLUDES})

add_library(scripts STATIC
  ${scripts_STAT_SRCS})

target_include_directories(scripts
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(scripts
  PRIVATE
    scripts-interface
    game-interface)

set_target_properties(scripts
  PROPERTIES
    FOLDER
      "server")

add_dependencies(scripts revision.h)

# Generate precompiled header
if (USE_SCRIPTPCH)
  add_cxx_pch(scripts ${PRIVATE_PCH_HEADER})
endif()
