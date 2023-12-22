cmake_minimum_required(VERSION 3.5)
project(tf2_broadcaster)

# for gazebo plugins
find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

# for ros2
find_package(rclcpp REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(gazebo_ros REQUIRED)

add_library(tf2_broadcaster SHARED tf2_broadcaster.cpp)
target_link_libraries(tf2_broadcaster ${GAZEBO_LIBRARIES})

ament_target_dependencies(tf2_broadcaster rclcpp geometry_msgs tf2 tf2_ros gazebo_ros)

ament_package()