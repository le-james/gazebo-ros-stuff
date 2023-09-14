cmake_minimum_required(VERSION 3.5)
# just a variable to reference to
# name of project doesn't matter but make it the name of the workspace to be clear
project(world_plugin_tutorial)

# gazebo says to add this but it doesn't seem to make a difference
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

# for gazebo plugins
find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

# create a subdirectory and adds basic_world_plugin (inside the world_plugins folder) into it
# basic_world_plugin is the name of the .so file - has to be the same name as the .cc file
add_subdirectory(world_plugins)
target_include_directories(basic_world_plugin PUBLIC world_plugins)

# print out paths to the terminal during cmake
# message("Path to GAZEBO_INCLUDE_DIRS: ${GAZEBO_INCLUDE_DIRS}")
# message("Path to GAZEBO_LIBRARY_DIRS: ${GAZEBO_LIBRARY_DIRS}")

# install(DIRECTORY 
#     models
# DESTINATION share/${PROJECT_NAME})