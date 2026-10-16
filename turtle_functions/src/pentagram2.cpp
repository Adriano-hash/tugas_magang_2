// ===== [1] INCLUDE =====
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

// ===== [2] CLASS: node adalah class yang mewarisi rclcpp::Node =====
class PentaGram2 : public rclcpp::Node
{
public:
  // ===== [3] CONSTRUCTOR: beri nama node, lalu daftarkan semua "pintu" ROS =====
  PentaGram2()
  : Node("pentagram_2")
  {
    // [3a] Parameter (opsional): nilai yang bisa diubah dari terminal
    this->declare_parameter("scalar", 1.0);
    scalar_ = this->get_parameter("scalar").as_double();

    // [3b] Publisher (opsional): mengirim ke topic
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    // [3c] Subscriber (opsional): menerima dari topic
    subscription_ = this->create_subscription<turtlesim::msg::Pose>(
      "/turtle1/pose", 10, std::bind(&PentaGram2::pose_callback, this, _1));

    // [3d] Timer (opsional): memanggil callback secara berkala
    timer_ = this->create_wall_timer(
      1000ms, std::bind(&PentaGram2::timer_callback, this));

    RCLCPP_INFO(this->get_logger(), "nama_node siap");
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

  void pose_callback(const turtlesim::msg::Pose & msg)
  {
    RCLCPP_INFO_THROTTLE(
      this->get_logger(), *this->get_clock(), 1000,
      "x=%.2f  y=%.2f  theta=%.2f rad", msg.x, msg.y, msg.theta);
  }

  // ===== [5] MEMBER: handle ROS dan data yang dibagi antar-callback =====
  double scalar_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription_;
  rclcpp::TimerBase::SharedPtr timer_;
};

// ===== [6] MAIN: titik masuk program =====
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PentaGram2>());
  rclcpp::shutdown();
  return 0;
}