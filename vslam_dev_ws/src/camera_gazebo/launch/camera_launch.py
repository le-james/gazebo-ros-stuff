import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch.actions import LogInfo, RegisterEventHandler, TimerAction
from launch.event_handlers import OnProcessStart

from launch_ros.actions import Node
import xacro

def generate_launch_description():

    # Specify the name of the package and path to xacro file within the package
    pkg_name = 'camera_gazebo'
    # file_subpath = 'description/camera.urdf.xacro'
    file_subpath = 'description/bu.xacro'


    # Use xacro to process the file
    xacro_file = os.path.join(get_package_share_directory(pkg_name),file_subpath)
    robot_description_raw = xacro.process_file(xacro_file).toxml()


    # Configure the node
    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[{'robot_description': robot_description_raw,
        'use_sim_time': True}] # add other parameters here if required
    )

    # gazebo node to launch (launches gazebo)
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
                os.path.join(get_package_share_directory('gazebo_ros'), 'launch', 'gazebo.launch.py')
            ]),
            # launch a custom world
            launch_arguments=
            {
                'world': './src/camera_gazebo/worlds/basic.world', 
                # 'pause': 'true'
                'pause': 'false'
            }.items()
    )

    # spawn the robot in gazebo using the urdf
    spawn_entity = Node(package='gazebo_ros', executable='spawn_entity.py',
                arguments=['-topic', 'robot_description',
                            '-entity', 'my_camera'],
                output='screen')
    
    rviz2 = Node(package='rviz2', 
                 executable='rviz2', 
                 name='rviz2', 
                 arguments=['-d',[os.path.join(get_package_share_directory(pkg_name), 'config', 'basic.rviz')]])

    static_world_link = Node(package='tf2_ros',
                             executable='static_transform_publisher',
                             arguments=['0', '0', '0', '0', '0', '0', 'world', 'base_link'])
                            #  arguments=['0', '0', '0', '0', '0', '0', 'world', 'slider_link'])
                            #  arguments=['0', '0', '0', '0', '0', '0', 'base_link', 'slider_link'])

    link_broadcaster = Node(package='camera_gazebo',
                            executable='link_broadcaster',
                            name='link_broadcaster_1')

    return LaunchDescription([

        # RegisterEventHandler only work for nodes not gazebo
        # RegisterEventHandler(
        #     event_handler=OnProcessStart(
        #     target_action=gazebo,
        #     on_start=[LogInfo(msg="Started the Gazebo. "), node_robot_state_publisher]
        #     # on_exit=[spawn_entity],
        # )),
        # RegisterEventHandler(
        #     event_handler=OnProcessStart(
        #     target_action=node_robot_state_publisher,
        #     on_start=[LogInfo(msg="Started the node_robot_state_publisher. "), spawn_entity]
        #     # on_exit=[spawn_entity],
        # )),
        # RegisterEventHandler(
        #     event_handler=OnProcessStart(
        #     target_action=spawn_entity,
        #     on_start=[LogInfo(msg="Started the spawn_entity. "), rviz2]
        #     # on_exit=[spawn_entity],
        # )),

        gazebo,
        node_robot_state_publisher,
        spawn_entity,
        static_world_link,
        # link_broadcaster,

        TimerAction(period=4.0,
            actions=[rviz2]),
    ])
        