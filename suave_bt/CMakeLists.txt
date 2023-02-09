cmake_minimum_required(VERSION 3.8)
project(suave_bt)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(behaviortree_cpp REQUIRED)
find_package(rclcpp REQUIRED )
# find_package(rclcpp_action REQUIRED )
find_package(rclcpp_lifecycle REQUIRED)
find_package(behaviortree_cpp REQUIRED)
# find_package(ament_index_cpp REQUIRED)

set(dependencies
  behaviortree_cpp
  rclcpp
  # rclcpp_action
  rclcpp_lifecycle
  std_msgs
  # std_srvs
)

set(library_name ${PROJECT_NAME})
set(executable_name bt)

include_directories(
  include
)

add_library(
  ${library_name} SHARED
  src/pipeline_detected_node.cpp
)

ament_target_dependencies(${library_name} ${dependencies})

add_executable(bt src/behavior_tree_engine.cpp)
ament_target_dependencies(${executable_name} ${dependencies})
target_link_libraries(${executable_name} ${library_name})


install(TARGETS ${executable_name}
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS ${library_name}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include/
)

ament_export_include_directories(include)
ament_export_libraries(${library_name})
ament_export_dependencies(${dependencies})
ament_package()


# if(BUILD_TESTING)
#   find_package(ament_lint_auto REQUIRED)
#   # the following line skips the linter which checks for copyrights
#   # comment the line when a copyright and license is added to all source files
#   set(ament_cmake_copyright_FOUND TRUE)
#   # the following line skips cpplint (only works in a git repo)
#   # comment the line when this package is in a git repo and when
#   # a copyright and license is added to all source files
#   set(ament_cmake_cpplint_FOUND TRUE)
#   ament_lint_auto_find_test_dependencies()
# endif()
