# CMakeLists.txt - Part of XTrism, (C) Daniel Wagenaar 2023

######################################################################
cmake_minimum_required(VERSION 3.22)

######################################################################
# Set the project name and version
project(XTrism VERSION 1.5.0)

set(CMAKE_PROJECT_HOMEPAGE_URL "http://danielwagenaar.net")

######################################################################
# Specify the C++ standard [must be done _before_ adding executable!]
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Qt stuff
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(PulseAudio)

find_package(Qt6
  COMPONENTS Widgets Multimedia Svg
  REQUIRED)

######################################################################
# Add the executables
add_executable(xtrism src/main/main.cpp)

include(src/CMakeLists.txt)

# Libraries to link to
target_link_libraries(xtrism Qt6::Widgets)
target_link_libraries(xtrism Qt6::Multimedia)
target_link_libraries(xtrism Qt6::Svg)
target_link_libraries(xtrism X11)
target_link_libraries(xtrism pulse)

# Include paths - Qt must be mentioned
target_include_directories(xtrism PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(xtrism PUBLIC "${Qt5_INCLUDE_DIRS}")
