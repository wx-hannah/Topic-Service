cmake_minimum_required(VERSION 3.8)
project(my_class_pkg)

# 默认编译模式
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ===================== 1. 依赖包查找 =====================
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rclcpp_action REQUIRED)    
find_package(rclcpp_components REQUIRED)

# ===================== 2. 自定义接口（msg + action） =====================
set(msg_files
  "msg/MyMessage.msg"
)
set(action_files
  "action/MyAction.action"
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  ${action_files}
  DEPENDENCIES
)

# ===================== 3. C++ 话题节点（保留，已有文件） =====================
add_executable(msg_publisher_node src/msg_publisher.cpp)
target_compile_features(msg_publisher_node PUBLIC c_std_99 cxx_std_17)
ament_target_dependencies(msg_publisher_node rclcpp)
rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME} rosidl_typesupport_cpp)
target_link_libraries(msg_publisher_node "${cpp_typesupport_target}")
install(TARGETS
  msg_publisher_node
  DESTINATION lib/${PROJECT_NAME})

add_executable(msg_subscriber_node src/msg_subscriber.cpp)
target_compile_features(msg_subscriber_node PUBLIC c_std_99 cxx_std_17)
ament_target_dependencies(msg_subscriber_node rclcpp)
target_link_libraries(msg_subscriber_node "${cpp_typesupport_target}")
install(TARGETS
  msg_subscriber_node
  DESTINATION lib/${PROJECT_NAME})

# ===================== 4. C++ Action 节点（仅当文件存在时编译） =====================
# 4.1 Action服务端（C++）- 检查文件是否存在
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/ros_action_server.cpp")
  add_executable(ros_action_server src/ros_action_server.cpp)
  target_compile_features(ros_action_server PUBLIC c_std_99 cxx_std_17)
  ament_target_dependencies(ros_action_server rclcpp rclcpp_action)
  target_link_libraries(ros_action_server "${cpp_typesupport_target}")
  install(TARGETS
    ros_action_server
    DESTINATION lib/${PROJECT_NAME})
endif()

# 4.2 Action客户端（C++）- 检查文件是否存在
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/ros_action_client.cpp")
  add_executable(ros_action_client src/ros_action_client.cpp)
  target_compile_features(ros_action_client PUBLIC c_std_99 cxx_std_17)
  ament_target_dependencies(ros_action_client rclcpp rclcpp_action)
  target_link_libraries(ros_action_client "${cpp_typesupport_target}")
  install(TARGETS
    ros_action_client
    DESTINATION lib/${PROJECT_NAME})
endif()

# ===================== 5. 其他文件安装 =====================
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/scripts")
  install(DIRECTORY
    scripts
    DESTINATION share/${PROJECT_NAME}/
  )
  install(PROGRAMS
    scripts/ros_action_server.py
    scripts/ros_action_client.py
    DESTINATION lib/${PROJECT_NAME}
  )
endif()

ament_package()

