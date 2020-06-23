# Package overloads
set(ACE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/deps/acelite)
set(ACE_LIBRARY "ace")

# check the CMake preload parameters (commented out by default)
# overload CMAKE_INSTALL_PREFIX if not being set properly
#if( WIN32 )
#  if( NOT CYGWIN )
#    if( NOT CMAKE_INSTALL_PREFIX )
#      set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/bin")
#    endif()
#  endif()
#endif()

if ( MSVC )
  include(${CMAKE_SOURCE_DIR}/src/cmake/compiler/msvc/settings.cmake)
elseif ( MINGW )
  include(${CMAKE_SOURCE_DIR}/src/cmake/compiler/mingw/settings.cmake)
endif()
