cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

project(libBatteryInfo
  VERSION 0.1.0
  LANGUAGES CXX
)

option(BUILD_STATIC_LIB "Build static library" ON)

option(BUILD_GUI_DEMO "Build GUI demo" OFF)

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

set(TARGET_LIBRARIES ${CMAKE_PROJECT_NAME})
add_library(${CMAKE_PROJECT_NAME} SHARED)

if(BUILD_STATIC_LIB)
  add_library(${CMAKE_PROJECT_NAME}Static STATIC)
  list(APPEND TARGET_LIBRARIES ${CMAKE_PROJECT_NAME}Static)
endif()

foreach(target ${TARGET_LIBRARIES})
  target_sources(${target} PRIVATE "${CXX_SOURCES}")
  target_compile_features(${target} PRIVATE cxx_std_23)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive) # /WX
    target_compile_options(${target} PRIVATE /utf-8) # /WX
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic) # -Werror
  endif()
  target_include_directories(${target} PUBLIC "inc/")
  if(WIN32)
    target_link_libraries(${target} PRIVATE setupapi)
  endif()
endforeach()

################################################################################
## Export

include(GNUInstallDirs)
install(TARGETS ${TARGET_LIBRARIES}
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

################################################################################
## Executables

if(BUILD_GUI_DEMO)
  add_subdirectory("gui")
endif()
