1.项目：Gazebo差速驱动机器人仿真项目
基于ROS2 humble 和 Gazebo Sim (Fortress)搭建的差速驱动机器人仿真环境，目标是实现完整的自主导航流程（建图、定位、路径规划）。

2.环境要求
- Ubuntu 22.04
- ROS2 Humble
- Gazebo Sim (ros-humble-ros-gz)
- Nav2 (ros-humble-navigation2, ros-humble-nav2-bringup)
- slam_toolbox (ros-humble-slam-toolbox)

3.技术栈
- ROS2 Humble（C++ / Python）
- Gazebo Sim (Fortress) / gz-sim
- ros_gz_bridge（ROS2与Gazebo话题桥接）
- slam_toolbox（SLAM建图）
- Nav2（导航栈：AMCL、Costmap、DWB局部规划器）
- Ubuntu 22.04


4.安装
git clone https://github.com/5683925/Gazebo.git
cd Gazebo
colcon build
source install/setup.bash

5.如何运行
ros2 launch my_robot_gazebo gazebo_sim.launch.py  #启动Gazebo仿真
ros2 run teleop_twist_keyboard teleop_twist_keyboard  #键盘控制机器人移动
ros2 launch my_robot_gazebo slam.launch.py #slam建图
ros2 run nav2_map_server map_saver_cli -f src/my_robot_gazebo/maps/my_map #保存地图
ros2 launch my_robot_gazebo navigation.launch.py #自主导航
rviz2 -d $(ros2 pkg prefix nav2_bringup)/share/nav2_bringup/rviz/nav2_default_view.rviz #查看rviz
ros2 run my_robot_behaviors simple_obstacle_avoider #C++简单避障节点

6.已知问题/环境注意事项
如果用虚拟机无法渲染机器人，可以尝试强制软件渲染：
export LIBGL_ALWAYS_SOFTWARE=1
每次打开导航后，需要先在rviz用2D pose后才能正常工作
在地图上机器人实际所在位置点击并拖动，松开确认绿色粒子群（AMCL定位估计）出现后，再点击 "Nav2 Goal" 设置目标点，机器人会自主规划路径并导航过去
这是AMCL的标准工作机制（`map → odom` 变换不会自动持久化），不是bug，每次重新启动都需要重复这一步。

7.整个项目建立步骤
7.1 机器人URDF建模（差速驱动底盘+双轮子+万向轮）
my_robot.urdf.xacro定义底盘轮子万向轮

7.2 RVIZ可视化与关节状态验证
display.launch.py启动 robot_state_publisher + joint_state_publisher_gui + rviz2 三个节点，在RViz里添加RobotModel显示插件，蓝色底盘+黑轮子+红色万向轮，并拖动滑条验证轮子joint角度变化。

7.3 Gazebo物理仿真集成，差速驱动插件
worlds/empty_world.sdf创建模拟世界
给机器人加物理驱动能力，在虚拟世界生成机器人，再让ROS2和Gazebo能互相通信

7.4 cmd_vel话题控制机器人移动
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist ...

7.5 键盘遥控
安装 ros-humble-teleop-twist-keyboard，用 i/j/k/l 按键控制机器人移动。





