# Topic-Service
my first experiment on class

wangxun‘s repository

# 5.1标准消息
cd ~/wx_ros_class_ws
# 先清理之前的编译缓存（可选，但建议执行）
colcon build --cmake-clean-first --packages-select my_class_pkg
# 加载环境变量
source install/setup.bash
#运行发布者命令
ros2 run my_class_pkg ros_publisher_node
#订阅话题：新建终端
source ~/wx_ros_class_ws/install/setup.bash
ros2 topic echo /my_topic


# 5.2 订阅话题
# 进入工作空间根目录
cd ~/wx_ros_class_ws
# 编译目标功能包（包含发布者+订阅者）
colcon build --packages-select my_class_pkg
# 加载环境变量
source install/setup.bash

#第一步，终端 发布者节点
source ~/wx_ros_class_ws/install/setup.bash
ros2 run my_class_pkg ros_publisher_node

#第二步，终端2 运行订阅者节点
source ~/wx_ros_class_ws/install/setup.bash
ros2 run my_class_pkg ros_subscriber_node

# 5.3自定义消息
# 5.3.1自定义消息发布
# 回到工作空间根目录（确保路径正确）
cd ~/wx_ros_class_ws
# 加载环境变量（必须执行，且要执行完整路径）
source install/setup.bash
# 再次运行节点
ros2 run my_class_pkg msg_publisher_node


# 5.3.2，运行自定义消息的订阅者节点
cd ~/wx_ros_class_ws
# 清理缓存（可选，确保编译最新代码）
colcon clean --packages-select my_class_pkg
# 编译功能包
colcon build --packages-select my_class_pkg
# 加载环境变量
source install/setup.bash
#建立新终端， 加载环境
source ~/wx_ros_class_ws/install/setup.bash
# 运行发布者节点
ros2 run my_class_pkg msg_publisher_node

# 5.5 实现 Launch 文件启动节点
ros2 launch my_class_pkg bringup_topic_launch.py

# 服务和动作
# 5.2实现ros动作的传递
#启动服务节点
source ~/wx_ros_class_ws/install/setup.bash
ros2 run my_class_pkg ros_action_server.py

#启动动作节点
source ~/wx_ros_class_ws/install/setup.bash
ros2 run my_class_pkg ros_action_client.py

