cmake_minimum_required(VERSION 3.10.2)
project(pipeline_inspection_missions)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(behaviortree_cpp REQUIRED)
find_package(Threads REQUIRED)

add_executable(pipeline_inspection_bt pipeline_inspection_BT.cpp)
target_link_libraries(pipeline_inspection_bt BT::behaviortree_cpp)
