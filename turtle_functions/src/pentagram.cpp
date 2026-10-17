#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class PentaGram : public rclcpp::Node
{
public:
  PentaGram()
  : Node("pentagram")
  {
    this->declare_parameter("scalar", 1.0);
    scalar_ = this->get_parameter("scalar").as_double();

    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    timer_ = this->create_wall_timer(
      1000ms, std::bind(&PentaGram::timer_callback, this));

    RCLCPP_INFO(this->get_logger(), "pentagram siap");
  }

private:
  bool active = false;
  void timer_callback()
  {
    geometry_msgs::msg::Twist msg;
    if(active == false) {
      msg.angular.z = -2.5125;
      active = true;
    } else {
      msg.linear.x = 1.0 * scalar_;
      active = false;
    }
    publisher_->publish(msg);
  }

  double scalar_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PentaGram>());
  rclcpp::shutdown();
  return 0;
}
