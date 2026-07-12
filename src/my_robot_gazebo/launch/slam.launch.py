import os
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    slam_toolbox_pkg = get_package_share_directory('slam_toolbox')
    gazebo_pkg = get_package_share_directory('my_robot_gazebo')
    params_file = os.path.join(gazebo_pkg, 'config', 'slam_params.yaml')

    slam = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(slam_toolbox_pkg, 'launch', 'online_async_launch.py')
        ),
        launch_arguments={
            'use_sim_time': 'true',
            'slam_params_file': params_file
        }.items()
    )

    return LaunchDescription([
        slam,
    ])
