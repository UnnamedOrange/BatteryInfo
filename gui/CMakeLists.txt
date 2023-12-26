cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

option(LINK_INSIGHT "Link Qt Insight Tracker library" ON)
option(BUILD_QDS_COMPONENTS "Build design studio components" ON)

set(CMAKE_AUTOMOC ON)

################################################################################
## Dependencies
include(FetchContent)

# Qt
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Qml Quick)

################################################################################
## Target

file(
  GLOB_RECURSE
  CXX_SOURCES
  CONFIGURE_DEPENDS
  "src/*"
)

if (Qt6_VERSION VERSION_GREATER_EQUAL 6.3)
  qt_standard_project_setup()
endif()
qt_add_executable(BatteryInfoApp "${CXX_SOURCES}")
qt_add_resources(BatteryInfoApp "configuration"
  PREFIX "/"
  FILES
    qtquickcontrols2.conf
)
target_compile_features(BatteryInfoApp PRIVATE cxx_std_23)
if(MSVC)
  target_compile_options(BatteryInfoApp PRIVATE /W4 /permissive) # /WX
  target_compile_options(BatteryInfoApp PRIVATE /utf-8) # /WX
else()
  target_compile_options(BatteryInfoApp PRIVATE -Wall -Wextra -Wpedantic) # -Werror
endif()
set_target_properties(BatteryInfoApp
  PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.orange.BatteryInfoApp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
target_link_libraries(BatteryInfoApp PRIVATE
  ${CMAKE_PROJECT_NAME}
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Qml
  Qt${QT_VERSION_MAJOR}::Quick
)

if (BUILD_QDS_COMPONENTS)
  include(${CMAKE_CURRENT_SOURCE_DIR}/qmlcomponents)
endif()
include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)
if (LINK_INSIGHT)
  include(${CMAKE_CURRENT_SOURCE_DIR}/insight)
endif ()

################################################################################
## Export

include(GNUInstallDirs)
install(TARGETS BatteryInfoApp
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
