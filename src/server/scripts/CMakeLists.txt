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

# Enable precompiled headers when using the GCC compiler.

CU_RUN_HOOK(BEFORE_SCRIPTS_LIBRARY)

CU_SET_PATH("WH_SCRIPTS_DIR" "${CMAKE_CURRENT_LIST_DIR}")

message(STATUS "SCRIPT PREPARATIONS")

macro(PrepareScripts name out)
  file(GLOB_RECURSE found
    ${name}/*.h
    ${name}/*.cpp)
  
  list(APPEND ${out} ${found})
  WH_ADD_DEF_SCRIPT_LOADER("${name}")
  WH_ADD_SCRIPT_LOADER("${name}" "")
  message(STATUS "  -> Prepared: ${name}")
endmacro(PrepareScripts)

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ScriptLoader.h
  ${BUILDDIR}/GenLoader.cpp)

CU_SET_PATH("CMAKE_WH_MODULE_DIR" "${CMAKE_CURRENT_LIST_DIR}")

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

CU_GET_GLOBAL("WH_ADD_SCRIPTS_LIST")
CU_GET_GLOBAL("WH_ADD_SCRIPTS_INCLUDE")

set("WH_SCRIPTS_INCLUDES" "")
set("WH_MODULE_LIST" "")
set("WH_SCRIPTS_LIST" "")

FOREACH (include ${WH_ADD_SCRIPTS_INCLUDE})
  set("WH_SCRIPTS_INCLUDES" "#include \"${include}\"\n${WH_SCRIPTS_INCLUDES}")
ENDFOREACH()

FOREACH (void ${WH_ADD_SCRIPTS_LIST})
  set("WH_MODULE_LIST" "void ${void};\n${WH_MODULE_LIST}")
ENDFOREACH()

FOREACH (scpript_list ${WH_DEF_SCRIPTS_LIST})
  set("WH_SCRIPTS_LIST" "${scpript_list};\n${WH_SCRIPTS_LIST}")
ENDFOREACH()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/GenLoader.cpp.cmake ${BUILDDIR}/GenLoader.cpp)

message(STATUS "SCRIPT PREPARATION COMPLETE")
message("")

if (USE_SCRIPTPCH)
  set(PRIVATE_PCH_HEADER PrecompiledHeaders/ScriptPCH.h)
endif ()

# Group sources
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_library(scripts STATIC
  ${scripts_STAT_SRCS})

target_include_directories(scripts
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(scripts
  PRIVATE
    acore-core-interface
  PUBLIC
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
