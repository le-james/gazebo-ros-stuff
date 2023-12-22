cmake_minimum_required(VERSION 3.5)
project(gazebo_ros_link_pose_publisher)

# for gazebo plugins
find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

# for ros2
find_package(rclcpp REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(gazebo_ros REQUIRED)

find_package(tf2_ros REQUIRED)

add_library(gazebo_ros_link_pose_publisher SHARED gazebo_ros_link_pose_publisher.cpp)
target_link_libraries(gazebo_ros_link_pose_publisher ${GAZEBO_LIBRARIES})

ament_target_dependencies(gazebo_ros_link_pose_publisher rclcpp geometry_msgs gazebo_ros tf2_ros)

ament_package()