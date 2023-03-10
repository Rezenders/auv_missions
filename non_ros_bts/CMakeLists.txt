cmake_minimum_required(VERSION 3.10.2)
project(pipeline_inspection_missions)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(behaviortree_cpp REQUIRED)
find_package(Threads REQUIRED)

add_executable(pipeline_inspection_bt suave/pipeline_inspection_BT.cpp)
target_link_libraries(pipeline_inspection_bt BT::behaviortree_cpp)

add_library(docking_nodes STATIC
    docking_bt/nodes/detect_marker_node.cpp
    docking_bt/nodes/dock_node.cpp
    docking_bt/nodes/home_node.cpp
)
target_link_libraries(docking_nodes PRIVATE BT::behaviortree_cpp)

add_executable(docking_bt docking_bt/docking_bt.cpp)
target_link_libraries(docking_bt BT::behaviortree_cpp docking_nodes)
