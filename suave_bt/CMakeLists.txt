cmake_minimum_required(VERSION 3.8)
project(suave_bt)

set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CONFIG_PATH ${CMAKE_MODULE_PATH}  "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CONFIG_PATH}")

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(behaviortree_cpp REQUIRED)
find_package(rclcpp REQUIRED )
# find_package(rclcpp_action REQUIRED )
find_package(rclcpp_lifecycle REQUIRED)
find_package(behaviortree_cpp REQUIRED)
find_package(rosa_msgs REQUIRED)
find_package(rosa_plan REQUIRED)
find_package(mavros_msgs REQUIRED)
# find_package(ament_index_cpp REQUIRED)

find_package(ZMQ)
if(ZMQ_FOUND)
    message(STATUS "ZeroMQ found.")
    add_definitions(-DZMQ_FOUND)
else()
  message(WARNING "ZeroMQ NOT found. Not including PublisherZMQ.")
endif()

set(dependencies
  behaviortree_cpp
  rclcpp
  # rclcpp_action
  rclcpp_lifecycle
  std_msgs
  mavros_msgs
  # std_srvs
  ament_index_cpp
  rosa_msgs
  rosa_plan
)

include_directories(
  include
  ${ZMQ_INCLUDE_DIRS}
)

add_library(is_pipeline_found_bt_node SHARED src/suave_bt/is_pipeline_found.cpp)
add_library(is_pipeline_inspected_bt_node SHARED src/suave_bt/is_pipeline_inspected.cpp)
add_library(is_enough_battery_bt_node SHARED src/suave_bt/is_enough_battery.cpp)
add_library(action_search_pipeline_bt_node SHARED src/suave_bt/action_search_pipeline.cpp)
add_library(action_inspect_pipeline_bt_node SHARED src/suave_bt/action_inspect_pipeline.cpp)
add_library(action_recharge_battery_bt_node SHARED src/suave_bt/action_recharge_battery.cpp)
add_library(arm_thrusters_bt_node SHARED src/suave_bt/arm_thrusters.cpp)
add_library(set_guided_mode_bt_node SHARED src/suave_bt/set_guided_mode.cpp)

list(APPEND plugin_libs
  is_pipeline_found_bt_node
  is_pipeline_inspected_bt_node
  is_enough_battery_bt_node
  action_search_pipeline_bt_node
  action_inspect_pipeline_bt_node
  action_recharge_battery_bt_node
  arm_thrusters_bt_node
  set_guided_mode_bt_node
)

foreach(bt_plugin ${plugin_libs})
  ament_target_dependencies(${bt_plugin} ${dependencies})
  target_compile_definitions(${bt_plugin} PRIVATE BT_PLUGIN_EXPORT)
endforeach()

add_executable(mock_suave src/mock_suave.cpp)
ament_target_dependencies(mock_suave ${dependencies})
target_link_libraries(mock_suave ${ZMQ_LIBRARIES} ${plugin_libs})


install(TARGETS
  mock_suave
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS ${plugin_libs}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include/
)

# add config, launch etc, whatever is needed
install(DIRECTORY bts
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


ament_export_include_directories(include)
# ament_export_libraries(${library_name})
ament_export_dependencies(${dependencies})
ament_package()
