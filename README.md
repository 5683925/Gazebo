1.项目：Gazebo差速驱动机器人仿真项目
基于ROS2 humble 和 Gazebo Sim (Fortress)搭建的差速驱动机器人仿真环境，目标是实现完整的自主导航流程（建图、定位、路径规划）。

2.环境要求
- Ubuntu 22.04
- ROS2 Humble
- Gazebo Sim (ros-humble-ros-gz)

3.安装
git clone https://github.com/你的用户名/仓库名.git
cd 仓库名
colcon build
source install/setup.bash

4.如何运行
ros2 launch my_robot_gazebo gazebo_sim.launch.py  #启动Gazebo仿真
ros2 run teleop_twist_keyboard teleop_twist_keyboard  #键盘控制机器人移动

5.已知问题/环境注意事项
如果用虚拟机无法渲染机器人，可以尝试强制软件渲染：
export LIBGL_ALWAYS_SOFTWARE=1

6.后续计划
加入激光雷达仿真、SLAM建图与NAV2导航能力，逐步实现完整的自主导航程序

7.整个项目建立步骤
7.1 机器人URDF建模（差速驱动底盘+双轮子+万向轮）
my_robot.urdf.xacro定义底盘轮子万向轮

7.2 RVIZ可视化与关节状态验证
display.launch.py启动了 robot_state_publisher + joint_state_publisher_gui + rviz2 三个节点，在RViz里加了RobotModel显示插件，看到了蓝色底盘+黑轮子+红色万向轮，还拖动滑条验证了轮子joint角度能变化。

7.3 Gazebo物理仿真集成，差速驱动插件
worlds/empty_world.sdf创建模拟世界
给机器人加物理驱动能力，生成机器人到世界里，再让ROS2和Gazebo能互相通信

7.4 cmd_vel话题控制机器人移动
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist ...

7.5 键盘遥控
装了 ros-humble-teleop-twist-keyboard，跑起来后用 i/j/k/l 按键控制机器人移动。





