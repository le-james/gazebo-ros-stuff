# pythons module
import os

# used to get the path to a directory
from ament_index_python.packages import get_package_share_directory

# used to create nodes
from launch_ros.actions import Node

# used to find directories outside of ~/foxy/lib
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument, ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource

# used to launch nodes
from launch import LaunchDescription

import xacro


# this i didn't include: output='screen' and use_sim_time substitution


# launch order: robot state publisher, gazebo, spawn entity
def generate_launch_description():

    # path to the robots xacro file
    my_gazebo_pkg = 'cart_pole_gazebo'
    my_cart_pole_xacro = 'cart_pole.urdf.xacro'
    path2xacro = os.path.join( get_package_share_directory(my_gazebo_pkg), 
                                'description', 
                               my_cart_pole_xacro )

    print('urdf_file_name : {}'.format(my_cart_pole_xacro))

    # combine all xacro files into one
    robot_description_config = xacro.process( path2xacro )

    # params = { 'robot_description': robot_description_config }

    # robot state publisher node to launch
    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='my_robot_state_publisher',    # name of the node
        parameters=[{ 'robot_description': robot_description_config }],
        # parameters=[params]
    )


    # gazebo node to launch (launches gazebo)
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [ os.path.join(get_package_share_directory('gazebo_ros'), 'launch', 'gazebo.launch.py')]),
            # launch a custom world
            launch_arguments={'world': './src/cart_pole_gazebo/worlds/world1.world'}.items()
    )

    # DOESN'T WORK
    # world = os.path.join(get_package_share_directory(my_gazebo_pkg), 'worlds', 'world1.world')
    # load_world = ExecuteProcess(cmd=['gazebo', '--verbose', '-s', 'libgazebo_ros_factory.so', world], output='screen' ),


    # spawns a robot in to gazebo
    spawn_entity = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=[ '-topic', 'robot_description', 
                    '-entity', 'cart_pole_cpp']
    )


    return LaunchDescription([
        # # alt way to launch a custom world
        # DeclareLaunchArgument(
        #     'world',
        #     # need to install the worlds directory in cmakelists.txt first before it will work
        #     # default_value=[os.path.join(get_package_share_directory(my_gazebo_pkg), 'worlds', 'world1.world')],
        #     # if provide the full path, it works without needing to install the worlds directory in cmakelists.txt
        #     default_value=['./src/cart_pole_gazebo/worlds/world1.world'],
        #     description='SDF world file'),

        robot_state_publisher_node,

        gazebo,

        # load_world,
        
        spawn_entity
    ])
