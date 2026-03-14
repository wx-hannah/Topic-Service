#include "rclcpp/rclcpp.hpp"
#include "my_class_pkg/srv/my_service_msg.hpp"
#include <memory>

using namespace std::chrono_literals;

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("my_service_client");

  // 创建客户端
  auto client = node->create_client<my_class_pkg::srv::MyServiceMsg>("my_service");

  // 等待服务可用
  while (!client->wait_for_service(1s)) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(node->get_logger(), "Interrupted while waiting for the service. Exiting.");
      return 0;
    }
    RCLCPP_INFO(node->get_logger(), "service not available, waiting again...");
  }

  // 创建请求
  auto request = std::make_shared<my_class_pkg::srv::MyServiceMsg::Request>();
  request->input = 42;

  // 异步发送请求
  auto result_future = client->async_send_request(request);

  // 等待结果 (在实际复杂应用中通常使用回调，这里为了演示简单使用 spin_until_future_complete)
  if (rclcpp::spin_until_future_complete(node, result_future) == rclcpp::FutureReturnCode::SUCCESS) {
    auto response = result_future.get();
    RCLCPP_INFO(node->get_logger(), "Service response: %ld", response->output);
  } else {
    RCLCPP_ERROR(node->get_logger(), "Failed to call service my_service");
  }

  rclcpp::shutdown();
  return 0;
}