// ===== [1] INCLUDE =====
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"   // ganti sesuai tipe pesan yang dipakai

using namespace std::chrono_literals;
using std::placeholders::_1;

// ===== [2] CLASS: node adalah class yang mewarisi rclcpp::Node =====
class PentaGram : public rclcpp::Node
{
public:
  // ===== [3] CONSTRUCTOR: beri nama node, lalu daftarkan semua "pintu" ROS =====
  PentaGram()
  : Node("pentagram"), count_(0)
  {
    // [3a] Parameter (opsional): nilai yang bisa diubah dari terminal
    this->declare_parameter("pesan", std::string("halo"));
    pesan_ = this->get_parameter("pesan").as_string();

    // [3b] Publisher (opsional): mengirim ke topic
    publisher_ = this->create_publisher<std_msgs::msg::String>("/turtle1/pose", 10);

    // [3d] Timer (opsional): memanggil callback secara berkala
    timer_ = this->create_wall_timer(
      500ms, std::bind(&PentaGram::timer_callback, this));

    RCLCPP_INFO(this->get_logger(), "pentagram siap");
  }

private:
  // ===== [4] CALLBACK: tempat logika node dijalankan =====
  void timer_callback()
  {
    auto message = std_msgs::msg::String();
    message.data = pesan_ + " " + std::to_string(count_++);
    publisher_->publish(message);
  }

  void subscriber_callback(const std_msgs::msg::String & msg)
  {
    RCLCPP_INFO(this->get_logger(), "Diterima: '%s'", msg.data.c_str());
  }

  // ===== [5] MEMBER: handle ROS dan data yang dibagi antar-callback =====
  std::string pesan_;
  size_t count_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
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