cmake_minimum_required(VERSION 3.5)
project(propeller_thrust_model_plugin)

# for gazebo plugins
find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

add_library(propeller_thrust_model_plugin SHARED propeller_thrust_model_plugin.cc)
target_link_libraries(propeller_thrust_model_plugin ${GAZEBO_LIBRARIES})
