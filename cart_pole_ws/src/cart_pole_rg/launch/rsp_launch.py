
import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.substitutions import LaunchConfiguration
from launch.actions import DeclareLaunchArgument
from launch_ros.actions import Node

import xacro


def generate_launch_description():

    # Check if we're told to use sim time
    use_sim_time = LaunchConfiguration('use_sim_time')

    # Process the URDF file
    pkg_path = os.path.join(get_package_share_directory('cart_pole_rg'))             # path to the /ddr package in /install of workspace
    xacro_file = os.path.join(pkg_path,'cart_pole.urdf.xacro')                    # path to robot xacro file
    robot_description_config = xacro.process_file(xacro_file)               # combine all URDF and xacro files into one
    
    # Create a robot_state_publisher node
    # params = {'robot_description': robot_description_config.toxml(), 'use_sim_time': use_sim_time}
    params = {'robot_description': robot_description_config.toxml(), 'use_sim_time': use_sim_time}

    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        # output='screen',
        parameters=[params]
    )
        # removing <output='screen'> doesn't seem to matter



    # Launch!
    return LaunchDescription([

        # arguments can be seen using the command <ros2 launch cart_pole_rg rsp_launch.py --show-args>
        DeclareLaunchArgument(
            'use_sim_time',
            default_value='false',
            description='Use sim time if true'),

        node_robot_state_publisher
    ])