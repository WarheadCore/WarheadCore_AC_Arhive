#
# Copyright (C) 2016+     AzerothCore <www.azerothcore.org>
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
