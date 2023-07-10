cmake_minimum_required(VERSION 3.8)
project(suave_bt)

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
  # std_srvs
  ament_index_cpp
)

include_directories(
  include
  ${ZMQ_INCLUDE_DIRS}
)

add_library(mock_pipeline_found_bt_node SHARED src/suave_bt/mock_pipeline_found.cpp)
add_library(mock_pipeline_inspected_bt_node SHARED src/suave_bt/mock_pipeline_inspected.cpp)
add_library(mock_enough_battery_bt_node SHARED src/suave_bt/mock_enough_battery.cpp)
add_library(mock_search_pipeline_bt_node SHARED src/suave_bt/mock_search_pipeline.cpp)
add_library(mock_inspect_pipeline_bt_node SHARED src/suave_bt/mock_inspect_pipeline.cpp)
add_library(mock_recharge_battery_bt_node SHARED src/suave_bt/mock_recharge_battery.cpp)

list(APPEND plugin_libs
  mock_pipeline_found_bt_node
  mock_pipeline_inspected_bt_node
  mock_enough_battery_bt_node
  mock_search_pipeline_bt_node
  mock_inspect_pipeline_bt_node
  mock_recharge_battery_bt_node
)

foreach(bt_plugin ${plugin_libs})
  ament_target_dependencies(${bt_plugin} ${dependencies})
  target_compile_definitions(${bt_plugin} PRIVATE BT_PLUGIN_EXPORT)
endforeach()

# ament_target_dependencies(${library_name} ${dependencies})

# add_executable(bt src/behavior_tree_engine.cpp)
# ament_target_dependencies(bt ${dependencies})
# target_link_libraries(bt ${ZMQ_LIBRARIES})

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