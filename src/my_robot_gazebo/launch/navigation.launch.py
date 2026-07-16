import os
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    nav2_bringup_pkg = get_package_share_directory('nav2_bringup')
    gazebo_pkg = get_package_share_directory('my_robot_gazebo')

    map_file = os.path.join(gazebo_pkg, 'maps', 'my_map.yaml')
    params_file = os.path.join(gazebo_pkg, 'config', 'nav2_params.yaml')

    navigation = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(nav2_bringup_pkg, 'launch', 'bringup_launch.py')
        ),
        launch_arguments={
            'map': map_file,
            'use_sim_time': 'true',
            'params_file': params_file,
        }.items()
    )

    return LaunchDescription([
        navigation,
    ])
