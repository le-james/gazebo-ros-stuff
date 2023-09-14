import os
from struct import pack

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch_ros.actions import Node


# new imports for launching controllers - some are unused

# from launch.actions import DeclareLaunchArgument
from launch.actions import ExecuteProcess
from launch.actions import RegisterEventHandler

from launch.event_handlers import OnProcessExit
# from launch.conditions import IfCondition
# from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():

    # Include the robot_state_publisher launch file, provided by our own package. Force sim time to be enabled
    # !!! MAKE SURE YOU SET THE PACKAGE NAME CORRECTLY !!!

    package_name='cart_pole_rg' #<--- CHANGE ME



    # IncludeLaunchDescription basically copy-pastes the code here from a path
    # includes external files to launch

    rsp = IncludeLaunchDescription(
                PythonLaunchDescriptionSource([os.path.join(
                    get_package_share_directory(package_name),'rsp_launch.py'
                )]), launch_arguments={'use_sim_time': 'true'}.items()
    )

    # Include the Gazebo launch file, provided by the gazebo_ros package
    # launch gazebo with ros integration
    gazebo = IncludeLaunchDescription(
                PythonLaunchDescriptionSource([os.path.join(
                    get_package_share_directory('gazebo_ros'),'launch','gazebo.launch.py')]),
             )


    # Run the spawner node from the gazebo_ros package. The entity name (cart_pole) doesn't really matter if you only have a single robot.
    # to spawn multiple robots we need to launch multiple robot state publishers?
    spawn_entity = Node(package='gazebo_ros', executable='spawn_entity.py',
                        arguments=['-topic', 'robot_description',
                                   '-entity', 'cart_pole'],
                        output='screen')
    # or can spawn by using a launch file
    # spawn_entity = IncludeLaunchDescription(
    #     PythonLaunchDescriptionSource(
    #         os.path.join(get_package_share_directory(package_name), "launch", "spawn launch file")
    #     )
    # )



    # launching controllers
    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner.py",
        arguments=["joint_state_broadcaster","--controller-manager","/controller_manager"]
    )

    # robot_controller_spawner = Node(
    #     package="controller_manager",
    #     executable="spawner.py",
    #     arguments=["joint_trajectory_controller","-c","/controller_manager"]
    # )

    load_effort_controller = ExecuteProcess(
        cmd=['ros2', 'control', 'load_controller', '--set-state', 'start', 'effort_controllers'],
        output='screen'
    )

    # Launch them all!
    return LaunchDescription([
        RegisterEventHandler(
            event_handler=OnProcessExit(
                target_action=spawn_entity,
                on_exit=[joint_state_broadcaster_spawner],
            )
        ),
        # RegisterEventHandler(
        #     event_handler=OnProcessExit(
        #         target_action=joint_state_broadcaster_spawner,
        #         on_exit=[robot_controller_spawner],
        #     )
        # ),
        RegisterEventHandler(
            event_handler=OnProcessExit(
                target_action=joint_state_broadcaster_spawner,
                on_exit=[load_effort_controller],
            )
        ),
        rsp,
        gazebo,
        spawn_entity,
    ])