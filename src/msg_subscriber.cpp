#include "rclcpp/rclcpp.hpp"
#include "my_class_pkg/msg/my_message.hpp"  // ROS 2自定义消息头文件

// 回调函数：收到消息时触发
void messageCallback(const my_class_pkg::msg::MyMessage::SharedPtr msg)
{
    // ROS 2日志输出（替换ROS 1的ROS_INFO）
    RCLCPP_INFO(
        rclcpp::get_logger("my_message_subscriber"), 
        "Received: key=%d, value='%s'", 
        msg->key, msg->value.c_str()
    );
}

int main(int argc, char * argv[])
{
    // 初始化ROS 2节点
    rclcpp::init(argc, argv);
    
    // 创建节点对象，命名为 "my_message_subscriber"
    auto node = std::make_shared<rclcpp::Node>("my_message_subscriber");
    
    // 创建订阅者：订阅话题 "/my_msg_topic"，队列大小10，指定回调函数
    auto subscriber = node->create_subscription<my_class_pkg::msg::MyMessage>(
        "/my_msg_topic", 10, messageCallback);
    
    // 打印启动日志
    RCLCPP_INFO(node->get_logger(), "Subscriber started, waiting for messages...");
    
    // 循环等待回调（替换ROS 1的ros::spin()）
    rclcpp::spin(node);
    
    // 关闭节点
    rclcpp::shutdown();
    return 0;
}

