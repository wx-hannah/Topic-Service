#include "rclcpp/rclcpp.hpp"
#include "my_class_pkg/srv/my_service_msg.hpp"
#include <memory>

using namespace std::chrono_literals;

class MyServiceServer : public rclcpp::Node
{
public:
  MyServiceServer()
  : Node("my_service_node")
  {
    // 创建服务
    // 注意：这里绑定了 _1 和 _2，对应下面的 request 和 response
    service_ = this->create_service<my_class_pkg::srv::MyServiceMsg>(
      "my_service", 
      std::bind(&MyServiceServer::handle_service, this, std::placeholders::_1, std::placeholders::_2));
    
    RCLCPP_INFO(this->get_logger(), "Ready to receive service requests.");
  }

private:
  // 【修改点】删除了 request_header 参数，只保留 request 和 response
  void handle_service(
    const std::shared_ptr<my_class_pkg::srv::MyServiceMsg::Request> request,
    std::shared_ptr<my_class_pkg::srv::MyServiceMsg::Response> response) // 注意：response 通常不需要 const
  {
    // 业务逻辑：输出 = 输入 * 2
    // 请确保你的 .srv 文件中定义的字段名确实是 input 和 output
    // 如果 .srv 定义是 int64 value 和 int64 result，请改为 request->value 和 response->result
    response->output = request->input * 2;
    
    RCLCPP_INFO(
      this->get_logger(), 
      "Request: input = %ld, output = %ld", 
      request->input, 
      response->output);
  }

  rclcpp::Service<my_class_pkg::srv::MyServiceMsg>::SharedPtr service_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<MyServiceServer>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
