cmake_minimum_required(VERSION 3.8)
project(turtle_functions)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)

add_executable(pentagram src/pentagram.cpp)
ament_target_dependencies(pentagram rclcpp geometry_msgs)

add_executable(pose_listener src/pose_listener.cpp)
ament_target_dependencies(pose_listener rclcpp turtlesim)

install(TARGETS
  pentagram
  pose_listener
  DESTINATION lib/${PROJECT_NAME})

ament_package()
