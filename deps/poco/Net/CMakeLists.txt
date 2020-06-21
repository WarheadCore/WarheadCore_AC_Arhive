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

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

add_library(Net ${SRCS})
add_library(Poco::Net ALIAS Net)

target_link_libraries(Net PUBLIC Poco::Foundation)

# Windows and WindowsCE need additional libraries
if(WIN32)
    target_link_libraries(Net PUBLIC "iphlpapi.lib")
    if(WINCE)
        target_link_libraries(Net PUBLIC "ws2.lib")
    else()
        target_link_libraries(Net PUBLIC "ws2_32.lib")
    endif()
endif(WIN32)

target_include_directories(Net
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(Net PROPERTIES LINKER_LANGUAGE CXX)
