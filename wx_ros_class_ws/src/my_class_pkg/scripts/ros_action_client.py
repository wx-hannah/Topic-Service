#!/usr/bin/env python3
import rclpy
from rclpy.action import ActionClient
from rclpy.node import Node
from my_class_pkg.action import MyAction

class MyActionClient(Node):
    def __init__(self):
        super().__init__('my_action_client')
        self._action_client = ActionClient(self, MyAction, 'my_action')

    def send_goal(self, object_name):
        self.get_logger().info('Waiting for action server to start...')
        if not self._action_client.wait_for_server(timeout_sec=10.0):
            self.get_logger().error('Action server not available!')
            return

        goal_msg = MyAction.Goal()
        goal_msg.object_name = object_name
        self.get_logger().info(f'Sending goal: object_name = {object_name}')

        self._send_goal_future = self._action_client.send_goal_async(
            goal_msg,
            feedback_callback=self.feedback_callback
        )
        self._send_goal_future.add_done_callback(self.goal_response_callback)

    def goal_response_callback(self, future):
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().error('Goal rejected by server!')
            return
        self.get_logger().info('Goal accepted by server, waiting for result...')
        self._get_result_future = goal_handle.get_result_async()
        self._get_result_future.add_done_callback(self.get_result_callback)

    def feedback_callback(self, feedback_msg):
        feedback = feedback_msg.feedback
        self.get_logger().info(f'Received feedback: progress = {feedback.progress:.1f}%')

    def get_result_callback(self, future):
        result = future.result().result
        status = future.result().status
        if status == 3:
            self.get_logger().info(f'Action finished successfully! Result: success = {result.success}')
        else:
            self.get_logger().warn(f'Action failed with status: {status}')
        rclpy.shutdown()

def main(args=None):
    rclpy.init(args=args)
    action_client = MyActionClient()
    action_client.send_goal("world")
    rclpy.spin(action_client)

if __name__ == '__main__':
    main()
