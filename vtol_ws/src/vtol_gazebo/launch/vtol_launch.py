import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():

    # gazebo node to launch (launches gazebo)
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
                os.path.join(get_package_share_directory('gazebo_ros'), 'launch', 'gazebo.launch.py')
            ]),
            # launch a custom world
            launch_arguments=
            {
                'world': './src/vtol_gazebo/worlds/model.world', 
                'pause': 'true'
            }.items()
    )

    return LaunchDescription([
        gazebo
    ])
        