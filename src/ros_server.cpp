#include "rclcpp/rclcpp.hpp"
// 注意头文件路径：包名/srv/服务名.hpp (ROS 2 默认生成 .hpp)
#include "my_class_pkg/srv/my_service_msg.hpp"
#include <memory>

using namespace std::chrono_literals;

class MyServiceServer : public rclcpp::Node
{
public:
  MyServiceServer()
  : Node("my_service_node")
  {
    // 创建服务，绑定回调函数
    service_ = this->create_service<my_class_pkg::srv::MyServiceMsg>(
      "my_service", 
      std::bind(&MyServiceServer::handle_service, this, std::placeholders::_1, std::placeholders::_2));
    
    RCLCPP_INFO(this->get_logger(), "Ready to receive service requests.");
  }

private:
  // 回调函数：request 是 const 引用，response 是共享指针引用
  void handle_service(
    const std::shared_ptr<rmw_request_id_t> request_header,
    const std::shared_ptr<my_class_pkg::srv::MyServiceMsg::Request> request,
    const std::shared_ptr<my_class_pkg::srv::MyServiceMsg::Response> response)
  {
    (void)request_header; // 未使用变量警告抑制
    
    // 业务逻辑：输出 = 输入 * 2
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