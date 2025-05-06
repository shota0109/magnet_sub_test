#include <wiringPi.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>  // Bool メッセージ

class RelayNode : public rclcpp::Node
{
public:
  RelayNode()
  : Node("relay_node")
  {
    // パラメータ：wiringPi 番号（デフォルト 0）
    this->declare_parameter<int>("wiringpi_pin", 0);
    this->get_parameter("wiringpi_pin", wiringpi_pin_);

    // wiringPi 初期化（BCM モードで扱いたい場合は wiringPiSetupGpio() に変更可）
    if (wiringPiSetup() == -1) {
      RCLCPP_FATAL(this->get_logger(), "wiringPiSetup failed");
      rclcpp::shutdown();
      return;
    }
    pinMode(wiringpi_pin_, OUTPUT);

    // magnet トピックをサブスクライブ
    sub_ = this->create_subscription<std_msgs::msg::Bool>(
      "magnet", 10,
      std::bind(&RelayNode::magnet_callback, this, std::placeholders::_1)
    );

    RCLCPP_INFO(this->get_logger(),
      "RelayNode started on WiringPi pin %d, subscribing to 'magnet'",
      wiringpi_pin_);
  }

private:
  void magnet_callback(const std_msgs::msg::Bool::SharedPtr msg)
  {
    // 受信したデータが true → ON、false → OFF
    bool state = msg->data;
    digitalWrite(wiringpi_pin_, state ? HIGH : LOW);
    RCLCPP_INFO(this->get_logger(),
      "Received magnet=%s → Relay %s",
      state ? "true" : "false",
      state ? "ON" : "OFF");
  }

  int wiringpi_pin_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RelayNode>());
  rclcpp::shutdown();
  return 0;
}
