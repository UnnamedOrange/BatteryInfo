cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

################################################################################
## Dependencies
include(FetchContent)

################################################################################
## Target

file(
  GLOB_RECURSE
  CXX_SOURCES
  CONFIGURE_DEPENDS
  "src/*"
)

add_executable(BatteryInfoCli "${CXX_SOURCES}")
target_compile_features(BatteryInfoCli PRIVATE cxx_std_23)
if(MSVC)
  target_compile_options(BatteryInfoCli PRIVATE /W4 /permissive-) # /WX
  target_compile_options(BatteryInfoCli PRIVATE /utf-8) # /WX
else()
  target_compile_options(BatteryInfoCli PRIVATE -Wall -Wextra -Wpedantic) # -Werror
endif()
set_target_properties(BatteryInfoCli
  PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.orange.BatteryInfoCli
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
)
target_link_libraries(BatteryInfoCli PRIVATE
  ${CMAKE_PROJECT_NAME}Static # For convenience, use static version to avoid cannot find DLL.
)

################################################################################
## Export

include(GNUInstallDirs)
install(TARGETS BatteryInfoCli
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
