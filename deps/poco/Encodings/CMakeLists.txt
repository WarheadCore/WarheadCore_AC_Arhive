#
# Copyright (C) 2016+     AzerothCore <www.azerothcore.org>
#

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES( SRCS Encodings ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS(SRCS Encodings ${HDRS_G})

add_library(Encodings ${SRCS})
add_library(Poco::Encodings ALIAS Encodings)

target_link_libraries(Encodings PUBLIC Poco::Foundation)
target_include_directories(Encodings
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(Encodings PROPERTIES LINKER_LANGUAGE CXX)
