cmake_minimum_required(VERSION 3.5)
project(model_plugin_tutorial)

# for gazebo plugins
find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

add_library(basic_model_plugin SHARED basic_model_plugin.cc)
target_link_libraries(basic_model_plugin ${GAZEBO_LIBRARIES})
