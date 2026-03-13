// 引入 ROS 2 核心头文件
#include "rclcpp/rclcpp.hpp"
// 引入标准字符串消息类型（ROS 2 版本）
#include "std_msgs/msg/string.hpp"

// 🔥 接收消息的回调函数（ROS 2 版本）
// 参数改为 std_msgs::msg::String::SharedPtr（ROS 2 推荐的智能指针）
void callback(const std_msgs::msg::String::SharedPtr msg)
{
    // 使用 RCLCPP_INFO 替代 ROS_INFO，绑定默认 logger（也可绑定节点 logger）
    RCLCPP_INFO(rclcpp::get_logger("my_subscriber"), "I heard: [%s]", msg->data.c_str());
}

int main(int argc, char *argv[])
{
    // 1. 初始化 ROS 2 上下文
    rclcpp::init(argc, argv);
    
    // 2. 创建 ROS 2 节点（名称为 "my_subscriber"，对应 ROS 1 的节点名）
    auto node = std::make_shared<rclcpp::Node>("my_subscriber");
    
    // 3. 定义订阅者对象
    // 参数1：订阅的话题名称 "my_topic"（和发布者保持一致）
    // 参数2：队列大小 10（和发布者保持一致）
    // 参数3：回调函数（ROS 2 直接传入函数名）
    auto subscriber = node->create_subscription<std_msgs::msg::String>(
        "my_topic", 10, callback);
    
    // 打印日志，提示订阅者已启动
    RCLCPP_INFO(node->get_logger(), "ROS 2 订阅者节点已启动，等待接收消息...");

    // 4. 运行节点（阻塞式，持续监听话题消息，替代 ROS 1 的 ros::spin()）
    rclcpp::spin(node);
    
    // 5. 关闭 ROS 2 上下文（spin 退出后执行）
    rclcpp::shutdown();
    return 0;
}

