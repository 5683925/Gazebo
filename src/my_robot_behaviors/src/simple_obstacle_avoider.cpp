#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <algorithm>
#include <cmath>

using std::placeholders::_1;

class SimpleObstacleAvoider : public rclcpp::Node
{
public:
  SimpleObstacleAvoider()
  : Node("simple_obstacle_avoider")
  {
    // 声明可调参数，方便不改代码就能调整避障行为
    this->declare_parameter("safe_distance", 0.6);      // 触发避障的距离阈值（米）
    this->declare_parameter("forward_speed", 0.2);       // 正常前进速度（m/s）
    this->declare_parameter("turn_speed", 0.5);          // 避障时的转向角速度（rad/s）
    this->declare_parameter("front_angle_range", 30.0);  // 检测“前方”的角度范围（度，左右各半）

    safe_distance_ = this->get_parameter("safe_distance").as_double();
    forward_speed_ = this->get_parameter("forward_speed").as_double();
    turn_speed_ = this->get_parameter("turn_speed").as_double();
    front_angle_range_ = this->get_parameter("front_angle_range").as_double();

    scan_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
      "/scan", rclcpp::SensorDataQoS(),
      std::bind(&SimpleObstacleAvoider::scan_callback, this, _1));

    cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    RCLCPP_INFO(this->get_logger(),
      "SimpleObstacleAvoider started. safe_distance=%.2fm, front_angle_range=%.1f deg",
      safe_distance_, front_angle_range_);
  }

private:
  void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
  {
    // 计算“正前方”对应扫描数组中的索引区间
    // LaserScan的angle_min到angle_max通常覆盖-π到π（或类似范围），0弧度代表正前方
    double half_range_rad = (front_angle_range_ / 2.0) * M_PI / 180.0;

    double min_front_distance = std::numeric_limits<double>::infinity();

    for (size_t i = 0; i < msg->ranges.size(); ++i)
    {
      double angle = msg->angle_min + i * msg->angle_increment;

      // 只关心正前方一个小角度范围内的扫描点
      if (angle >= -half_range_rad && angle <= half_range_rad)
      {
        double range = msg->ranges[i];
        // 过滤掉无效值（inf、nan、超出量程）
        if (std::isfinite(range) && range >= msg->range_min && range <= msg->range_max)
        {
          min_front_distance = std::min(min_front_distance, static_cast<double>(range));
        }
      }
    }

    geometry_msgs::msg::Twist cmd;

    if (min_front_distance < safe_distance_)
    {
      // 前方有障碍物，停止前进，原地转向避开
      cmd.linear.x = 0.0;
      cmd.angular.z = turn_speed_;
      RCLCPP_INFO(this->get_logger(),
        "Obstacle detected at %.2fm (< %.2fm threshold). Turning to avoid.",
        min_front_distance, safe_distance_);
    }
    else
    {
      // 前方安全，正常前进
      cmd.linear.x = forward_speed_;
      cmd.angular.z = 0.0;
    }

    cmd_vel_pub_->publish(cmd);
  }

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;

  double safe_distance_;
  double forward_speed_;
  double turn_speed_;
  double front_angle_range_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SimpleObstacleAvoider>());
  rclcpp::shutdown();
  return 0;
}
