// ===== [1] INCLUDE =====
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

// ===== [2] CLASS: node adalah class yang mewarisi rclcpp::Node =====
class PentaGram : public rclcpp::Node
{
public:
  // ===== [3] CONSTRUCTOR: beri nama node, lalu daftarkan semua "pintu" ROS =====
  PentaGram()
  : Node("pentagram")
  {
    // [3a] Parameter (opsional): nilai yang bisa diubah dari terminal
    this->declare_parameter("scalar", 1.0);
    scalar_ = this->get_parameter("scalar").as_double();

    // [3b] Publisher (opsional): mengirim ke topic
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    // [3d] Timer (opsional): memanggil callback secara berkala
    timer_ = this->create_wall_timer(
      1000ms, std::bind(&PentaGram::timer_callback, this));

    RCLCPP_INFO(this->get_logger(), "pentagram siap");
  }

private:
  // ===== [4] CALLBACK: tempat logika node dijalankan =====
  bool active = false;
  void timer_callback()
  {
    geometry_msgs::msg::Twist msg;
    if(active == false) {
      msg.angular.z = -2.51;
      active = true;
    } else {
      msg.linear.x = 1.0 * scalar_;
      active = false;
    }
    publisher_->publish(msg);
  }

  // ===== [5] MEMBER: handle ROS dan data yang dibagi antar-callback =====
  double scalar_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

// ===== [6] MAIN: titik masuk program =====
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PentaGram>());
  rclcpp::shutdown();
  return 0;
}