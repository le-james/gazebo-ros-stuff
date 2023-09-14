cmake_minimum_required(VERSION 3.5)
project(subscriber_plugin_tutorial)

# for gazebo plugins
find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

add_library(basic_sub_plugin SHARED basic_sub_plugin.cc)
target_link_libraries(basic_sub_plugin ${GAZEBO_LIBRARIES})
