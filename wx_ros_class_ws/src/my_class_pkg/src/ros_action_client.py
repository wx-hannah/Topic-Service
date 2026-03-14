#!/usr/bin/env python3
import rclpy
from rclpy.action import ActionClient
from rclpy.node import Node
# 导入自定义 Action 类型
from my_class_pkg.action import MyAction

class MyActionClient(Node):
    def __init__(self):
        super().__init__('my_action_client')
        # 创建 Action 客户端（对应 ROS 1 的 SimpleActionClient）
        self._action_client = ActionClient(self, MyAction, 'my_action')

    def send_goal(self, object_name):
        """发送目标到服务端，并处理反馈/结果"""
        # 等待服务端启动（对应 ROS 1 的 waitForServer()）
        self.get_logger().info('Waiting for action server to start...')
        if not self._action_client.wait_for_server(timeout_sec=10.0):
            self.get_logger().error('Action server not available!')
            return

        # 创建目标请求（对应 ROS 1 的 MyActionGoal）
        goal_msg = MyAction.Goal()
        goal_msg.object_name = object_name  # 设置目标参数

        self.get_logger().info(f'Sending goal: object_name = {object_name}')

        # 发送目标，并注册反馈/结果回调
        self._send_goal_future = self._action_client.send_goal_async(
            goal_msg,
            feedback_callback=self.feedback_callback  # 反馈回调
        )
        # 注册结果回调
        self._send_goal_future.add_done_callback(self.goal_response_callback)

    def goal_response_callback(self, future):
        """处理服务端的目标响应"""
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().error('Goal rejected by server!')
            return

        self.get_logger().info('Goal accepted by server, waiting for result...')
        # 等待最终结果
        self._get_result_future = goal_handle.get_result_async()
        self._get_result_future.add_done_callback(self.get_result_callback)

    def feedback_callback(self, feedback_msg):
        """处理服务端的实时反馈"""
        feedback = feedback_msg.feedback
        self.get_logger().info(f'Received feedback: progress = {feedback.progress:.1f}%')

    def get_result_callback(self, future):
        """处理最终结果"""
        result = future.result().result
        status = future.result().status

        if status == 3:  # SUCCEEDED
            self.get_logger().info(f'Action finished successfully! Result: success = {result.success}')
        else:
            self.get_logger().warn(f'Action failed with status: {status}')

        # 关闭节点
        rclpy.shutdown()

def main(args=None):
    # 初始化 ROS 2
    rclpy.init(args=args)
    # 创建客户端节点
    action_client = MyActionClient()
    # 发送目标（对应 ROS 1 的 goal.object_name = "world"）
    action_client.send_goal("world")
    # 保持节点运行
    rclpy.spin(action_client)

if __name__ == '__main__':
    main()

