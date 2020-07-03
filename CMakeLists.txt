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

# Set projectname (must be done AFTER setting configurationtypes)
project(WarheadCore)

# Require a min version Cmake
cmake_minimum_required(VERSION 3.8)

# CMake policies (can not be handled elsewhere)
cmake_policy(SET CMP0005 NEW)

if (CMAKE_VERSION VERSION_LESS "3.16.0")
  cmake_policy(SET CMP0043 OLD) # Disable 'Ignore COMPILE_DEFINITIONS_<Config> properties'
else()
  cmake_policy(SET CMP0043 NEW) # Cotire isn't used so set to NEW
endif()

cmake_policy(SET CMP0054 NEW) # Only interpret if() arguments as variables or keywords when unquoted - prevents intepreting if (SOME_STRING_VARIABLE MATCHES "MSVC") as if (SOME_STRING_VARIABLE MATCHES "1")

# add this options before PROJECT keyword
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Set RPATH-handing (CMake parameters)
set(CMAKE_SKIP_BUILD_RPATH 0)
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

set(AC_PATH_ROOT "${CMAKE_SOURCE_DIR}")

# set macro-directory
list(APPEND CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/src/cmake/macros")

if (CMAKE_VERSION VERSION_LESS "3.16.0")
  list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/deps/cotire/CMake")
endif()

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)

# some utils for cmake
include(deps/drassil/cmake-utils/utils.cmake)

include(src/cmake/ac_macros.cmake)

# set default buildoptions and print them
include(conf/config.cmake.dist)

# load custom configurations for cmake if exists
if(EXISTS "${CMAKE_SOURCE_DIR}/conf/config.cmake")
  include(conf/config.cmake)
endif()

# Load modules
include(modules/LoadingModules.cmake)

CU_RUN_HOOK("AFTER_LOAD_CONF")

# build in Release-mode by default if not explicitly set
if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "Release")
endif()

# turn off PCH totally if enabled (hidden setting, mainly for devs)
if( NOPCH )
  set(USE_COREPCH 0)
  set(USE_SCRIPTPCH 0)
endif()

include(ConfigureBaseTargets)
include(CheckPlatform)
include(GroupSources)
include(AutoCollect)
include(PocoMacros)

CU_RUN_HOOK("AFTER_LOAD_CMAKE_MODULES")

# basic packagesearching and setup (further support will be needed, this is a preliminary release!)
set(ACE_EXPECTED_VERSION 6.0.3)

find_package(PCHSupport)
find_package(ACE REQUIRED)
find_package(MySQL REQUIRED)

if(UNIX AND WITH_PERFTOOLS)
  find_package(Gperftools)
endif()

if(NOT WITHOUT_GIT)
  find_package(Git)
endif()

# Find revision ID and hash of the sourcetree
include(src/cmake/genrev.cmake)

# print out the results before continuing
include(src/cmake/showoptions.cmake)

#
# Loading framework
#

add_subdirectory(deps)

if( SERVERS OR TOOLS)
  add_subdirectory(src/common)
endif()

if( TOOLS )
  add_subdirectory(src/tools)
endif()

#
# Loading application sources
#

CU_RUN_HOOK("BEFORE_SRC_LOAD")

# add core sources
add_subdirectory(src)

# add modules
add_subdirectory(modules)

CU_RUN_HOOK("AFTER_SRC_LOAD")
