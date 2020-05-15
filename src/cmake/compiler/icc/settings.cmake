#
# This file is part of the WarheadCore Project. See AUTHORS file for Copyright information
#

# Set build-directive (used in core to tell which buildtype we used)
target_compile_definitions(acore-compile-option-interface
  INTERFACE
    -D_BUILD_DIRECTIVE="${CMAKE_BUILD_TYPE}")

if(PLATFORM EQUAL 32)
  target_compile_options(acore-compile-option-interface
    INTERFACE
      -axSSE2)
else()
  target_compile_options(acore-compile-option-interface
    INTERFACE
      -xSSE2)
endif()

if(WITH_WARNINGS)
  target_compile_options(acore-warning-interface
    INTERFACE
      -w1)
  message(STATUS "ICC: All warnings enabled")
endif()

if(WITH_COREDEBUG)
  target_compile_options(acore-compile-option-interface
    INTERFACE
      -g)
  message(STATUS "ICC: Debug-flag set (-g)")
endif()
