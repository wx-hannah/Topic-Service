#include "rclcpp/rclcpp.hpp"
#include "my_class_pkg/msg/my_message.hpp"  // ROS 2的消息头文件路径格式

int main(int argc, char * argv[])
{
    // 初始化ROS 2节点
    rclcpp::init(argc, argv);
    
    // 创建节点对象，命名为 "my_message_publisher"
    auto node = std::make_shared<rclcpp::Node>("my_message_publisher");
    
    // 创建发布者，话题名为 "/my_msg_topic"，队列大小 10
    auto publisher = node->create_publisher<my_class_pkg::msg::MyMessage>(
        "/my_msg_topic", 10);
    
    // 设置发布频率（1 Hz）
    rclcpp::Rate rate(1);
    
    int key = 0;
    
    // 节点运行循环
    while (rclcpp::ok())
    {
        // 创建消息对象并赋值
        my_class_pkg::msg::MyMessage msg;
        msg.key = key;
        msg.value = "Hello from ROS2 C++ publisher, key = " + std::to_string(key);
        
        // 打印日志（ROS 2的日志接口）
        RCLCPP_INFO(node->get_logger(), "Publishing: key=%d, value='%s'", 
                     msg.key, msg.value.c_str());
        
        // 发布消息
        publisher->publish(msg);
        
        // 自增计数
        ++key;
        
        // 按照设定频率休眠
        rate.sleep();
    }
    
    // 关闭ROS 2节点
    rclcpp::shutdown();
    return 0;
}

