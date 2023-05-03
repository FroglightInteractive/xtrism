# CMakeLists.txt - Part of XTrism, (C) Daniel Wagenaar 2023

######################################################################
cmake_minimum_required(VERSION 3.16)

######################################################################
# Set the project name and version
project(XTrism VERSION 1.5.0)

set(CMAKE_PROJECT_HOMEPAGE_URL "http://danielwagenaar.net")

######################################################################
# Specify the C++ standard [must be done _before_ adding executable!]
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON) # I thought this would do the trick...
#if (UNIX) 
#  add_compile_options(-fPIC) # ... but in fact, this is required on Linux
#endif()

# Qt stuff
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6
  COMPONENTS Widgets Multimedia 
  REQUIRED)

######################################################################
# Add the executables
add_executable(xtrism src/main/main.cpp)

include(src/CMakeLists.txt)

# Libraries to link to - Qt must be mentioned
target_link_libraries(xtrism Qt6::Widgets)
target_link_libraries(xtrism Qt6::Multimedia)
target_link_libraries(xtrism X11)

# Include paths - Qt must be mentioned
target_include_directories(xtrism PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(xtrism PUBLIC "${Qt6_INCLUDE_DIRS}")
