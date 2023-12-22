cmake_minimum_required(VERSION 3.5)
project(model_plugin_ros_tutorial)

# for gazebo plugins
find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

# for ros2
find_package(rclcpp REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(gazebo_ros REQUIRED)

add_library(basic_model_plugin_ros SHARED basic_model_plugin_ros.cc)
target_link_libraries(basic_model_plugin_ros ${GAZEBO_LIBRARIES})

ament_target_dependencies(basic_model_plugin_ros rclcpp std_msgs gazebo_ros)

ament_package()
