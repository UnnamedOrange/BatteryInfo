cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

project(libBatteryInfo
  VERSION 0.1.0
  LANGUAGES CXX
)

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
  "inc/*"
)

add_library(${CMAKE_PROJECT_NAME} "${CXX_SOURCES}")
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_23)
if(MSVC)
  target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4 /permissive) # /WX
  target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /utf-8) # /WX
else()
  target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic) # -Werror
endif()
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "inc/")
if(WIN32)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    setupapi
  )
endif()

################################################################################
## Export

include(GNUInstallDirs)
install(TARGETS ${CMAKE_PROJECT_NAME}
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

################################################################################
## Executables

add_subdirectory("gui")
