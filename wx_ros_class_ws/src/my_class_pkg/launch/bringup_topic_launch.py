# 必须的导入（缺一不可）
from launch import LaunchDescription
from launch_ros.actions import Node

# 核心函数：名称必须是 generate_launch_description（不能改）
def generate_launch_description():
    # 定义发布者节点（缩进4个空格，Python 语法要求）
    publisher_node = Node(
        package='my_class_pkg',
        executable='msg_publisher_node',
        name='my_message_publisher',
        output='screen'
    )

    # 定义订阅者节点
    subscriber_node = Node(
        package='my_class_pkg',
        executable='msg_subscriber_node',
        name='my_message_subscriber',
        output='screen'
    )

    # 返回启动描述（包含两个节点）
    return LaunchDescription([
        publisher_node,
        subscriber_node
    ])

