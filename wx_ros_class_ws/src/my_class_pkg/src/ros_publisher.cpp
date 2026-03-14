// 引入 ROS 2 核心头文件
#include "rclcpp/rclcpp.hpp"
// 引入标准字符串消息类型（ROS 2 版本）
#include "std_msgs/msg/string.hpp"

int main(int argc, char *argv[])
{
    // 1. 初始化 ROS 2 上下文
    rclcpp::init(argc, argv);
    
    // 2. 创建 ROS 2 节点（名称为 "my_publisher"，对应 ROS 1 的节点名）
    // ROS 2 无需 NodeHandle，直接通过节点对象操作
    auto node = std::make_shared<rclcpp::Node>("my_publisher");
    
    // 3. 创建发布者对象
    // 参数1：话题名称 "my_topic"（和 ROS 1 保持一致）
    // 参数2：队列大小 10（和 ROS 1 保持一致）
    auto publisher = node->create_publisher<std_msgs::msg::String>("my_topic", 10);
    
    // 4. 定义发布频率（1Hz，间隔 1 秒，对应 ROS 1 的 ros::Rate(1.0)）
    rclcpp::Rate rate(1.0);
    
    // 打印日志（ROS 2 推荐用节点的 logger，更清晰）
    RCLCPP_INFO(node->get_logger(), "ROS 2 发布者节点已启动，每秒发布一次消息...");

    // 5. 循环发布消息（直到节点停止）
    while (rclcpp::ok())
    {
        // 创建消息对象并填充数据
        std_msgs::msg::String msg;
        msg.data = "Hello, world! (ROS 2 Version)";
        
        // 发布消息
        publisher->publish(msg);
        RCLCPP_INFO(node->get_logger(), "已发布消息：%s", msg.data.c_str());
        // 打印调试信息（可选）
        RCLCPP_DEBUG(node->get_logger(), "发布消息：%s", msg.data.c_str());
        
        // 按频率休眠（1 秒）
        rate.sleep();
    }

    // 6. 关闭 ROS 2 上下文
    rclcpp::shutdown();
    return 0;
}

