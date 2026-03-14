#!/usr/bin/env python3
import rclpy
from rclpy.action import ActionServer
from rclpy.node import Node
# 导入自定义 Action 类型（注意 ROS 2 导入路径）
from my_class_pkg.action import MyAction

class MyActionServer(Node):
    def __init__(self):
        super().__init__('my_action_server')
        # 创建 Action 服务端（对应 ROS 1 的 SimpleActionServer）
        self._action_server = ActionServer(
            self,
            MyAction,          # 自定义 Action 类型
            'my_action',       # Action 名称（和客户端一致）
            self.execute_callback  # 执行回调函数
        )
        self.get_logger().info('Python Action Server started, waiting for goals...')

    def execute_callback(self, goal_handle):
        """核心执行回调：处理客户端的 goal，返回 feedback 和 result"""
        self.get_logger().info(f'Received goal request: object_name = {goal_handle.request.object_name}')
        
        # 初始化 feedback 和 result
        feedback_msg = MyAction.Feedback()
        result = MyAction.Result()

        # 模拟动作执行（10次循环，1Hz频率，进度从10%到100%）
        for i in range(1, 11):
            # 检查是否被取消（对应 ROS 1 的 isPreemptRequested()）
            if goal_handle.is_cancel_requested:
                goal_handle.canceled()
                self.get_logger().info('Action canceled by client')
                result.success = False
                return result
            
            # 更新进度反馈（对应 ROS 1 的 feedback_.progress）
            feedback_msg.progress = i * 10.0
            self.get_logger().info(f'Executing, progress = {feedback_msg.progress:.1f}%')
            # 发布反馈（对应 ROS 1 的 publishFeedback()）
            goal_handle.publish_feedback(feedback_msg)

            # 休眠1秒（1Hz频率）
            rclpy.spin_once(self, timeout_sec=1.0)

        # 执行完成，标记成功（对应 ROS 1 的 setSucceeded()）
        goal_handle.succeed()
        result.success = True
        self.get_logger().info('Action succeeded!')
        return result

def main(args=None):
    # 初始化 ROS 2
    rclpy.init(args=args)
    # 创建服务端节点
    action_server = MyActionServer()
    # 保持节点运行
    rclpy.spin(action_server)
    # 关闭
    action_server.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

