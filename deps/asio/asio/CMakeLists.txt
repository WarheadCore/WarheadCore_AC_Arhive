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

# CollectSourceFiles(
#   ${CMAKE_CURRENT_SOURCE_DIR}
#   PRIVATE_SOURCES)

# Group sources
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

# Create static Library
add_library(asio INTERFACE)

CollectIncludeDirectories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC_INCLUDES)

target_include_directories(asio
  INTERFACE
    ${PUBLIC_INCLUDES})

target_compile_definitions(asio
  INTERFACE
    ASIO_HAS_BOOST_DATE_TIME)

# set_target_properties(asio
#   PROPERTIES
#     FOLDER
#       "deps")
