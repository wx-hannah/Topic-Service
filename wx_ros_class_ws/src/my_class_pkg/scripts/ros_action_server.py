#!/usr/bin/env python3
import rclpy
from rclpy.action import ActionServer
from rclpy.node import Node
from my_class_pkg.action import MyAction

class MyActionServer(Node):
    def __init__(self):
        super().__init__('my_action_server')
        self._action_server = ActionServer(
            self,
            MyAction,
            'my_action',
            self.execute_callback
        )
        self.get_logger().info('Python Action Server started, waiting for goals...')

    def execute_callback(self, goal_handle):
        self.get_logger().info(f'Received goal request: object_name = {goal_handle.request.object_name}')
        feedback_msg = MyAction.Feedback()
        result = MyAction.Result()

        for i in range(1, 11):
            if goal_handle.is_cancel_requested:
                goal_handle.canceled()
                self.get_logger().info('Action canceled by client')
                result.success = False
                return result
            feedback_msg.progress = i * 10.0
            self.get_logger().info(f'Executing, progress = {feedback_msg.progress:.1f}%')
            goal_handle.publish_feedback(feedback_msg)
            rclpy.spin_once(self, timeout_sec=1.0)

        goal_handle.succeed()
        result.success = True
        self.get_logger().info('Action succeeded!')
        return result

def main(args=None):
    rclpy.init(args=args)
    action_server = MyActionServer()
    rclpy.spin(action_server)
    action_server.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
