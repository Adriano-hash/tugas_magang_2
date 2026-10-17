#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <memory>

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class PentaGram2 : public rclcpp::Node
{
public:
  PentaGram2()
  : Node("pentagram_2")
  {
    this->declare_parameter("size", 1.0);
    size_ = this->get_parameter("size").as_double();
    this->declare_parameter("angular_scalar", 1.0);
    angular_scalar_ = this->get_parameter("angular_scalar").as_double();

    publisher_ =
      this->create_publisher<geometry_msgs::msg::Twist>(
      "/turtle1/cmd_vel", 10);

    subscription_ =
      this->create_subscription<turtlesim::msg::Pose>(
      "/turtle1/pose", 10,
      std::bind(&PentaGram2::pose_callback, this, _1));

    timer_ =
      this->create_wall_timer(
      50ms, std::bind(&PentaGram2::timer_callback, this));

    RCLCPP_INFO(this->get_logger(), "pentagram_2 siap");
  }

private:
  enum class MotionState
  {
    TURNING,
    MOVING
  };

  static constexpr double pi_ = 3.14159265358979323846;
  static constexpr double target_turn_ = 4.0 * pi_ / 5.0;
  static constexpr double target_side_length_ = 1.0;
  static constexpr double max_angular_speed_ = 1.2;
  static constexpr double turn_gain_ = 3.0;
  static constexpr double turn_tolerance_ = 0.005;
  static constexpr double minimum_turn_speed_ = 0.10;
  static constexpr double control_period_seconds_ = 0.05;

  void pose_callback(const turtlesim::msg::Pose & msg)
  {
    current_pose_ = msg;

    if (!pose_ready_) {
      state_start_pose_ = msg;
      pose_ready_ = true;
    }

    RCLCPP_INFO_THROTTLE(
      this->get_logger(), *this->get_clock(), 1000,
      "x=%.2f  y=%.2f  theta=%.2f rad",
      msg.x, msg.y, msg.theta);
  }

  static double normalize_angle(double angle)
  {
    while (angle > pi_) {
      angle -= 2.0 * pi_;
    }

    while (angle < -pi_) {
      angle += 2.0 * pi_;
    }

    return angle;
  }

  static double distance_between(
    const turtlesim::msg::Pose & first,
    const turtlesim::msg::Pose & second)
  {
    const double dx = first.x - second.x;
    const double dy = first.y - second.y;
    return std::hypot(dx, dy);
  }

  void timer_callback()
  {
    geometry_msgs::msg::Twist command;

    if (!pose_ready_) {
      publisher_->publish(command);
      return;
    }

    if (state_ == MotionState::TURNING) {
      const double angle_difference = normalize_angle(
        current_pose_.theta - state_start_pose_.theta);
      const double angle_turned = std::fabs(angle_difference);

      if (angle_turned + turn_tolerance_ < target_turn_) {
        const double remaining_angle = target_turn_ - angle_turned;
        const double requested_speed = turn_gain_ * remaining_angle;
        const double maximum_speed = max_angular_speed_ * angular_scalar_;
        const double speed_before_final_step = std::min(
          std::max(requested_speed, minimum_turn_speed_),
          maximum_speed);
        const double turn_speed = std::min(
          speed_before_final_step,
          remaining_angle / control_period_seconds_);
        command.angular.z = -turn_speed;
      } else {
        state_ = MotionState::MOVING;
        state_start_pose_ = current_pose_;
      }
    } else if (state_ == MotionState::MOVING) {
      const double distance_moved = distance_between(
        current_pose_, state_start_pose_);

      if (distance_moved < target_side_length_*size_) {
        const double remaining_distance = target_side_length_ * size_ - distance_moved;
        command.linear.x = std::min(
          size_,
          remaining_distance / control_period_seconds_);
      } else {
        state_ = MotionState::TURNING;
        state_start_pose_ = current_pose_;
      }
    }

    publisher_->publish(command);
  }

  double size_ = 1.0;
  double angular_scalar_ = 1.0;
  bool pose_ready_ = false;
  MotionState state_ = MotionState::TURNING;

  turtlesim::msg::Pose current_pose_;
  turtlesim::msg::Pose state_start_pose_;

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PentaGram2>());
  rclcpp::shutdown();
  return 0;
}
