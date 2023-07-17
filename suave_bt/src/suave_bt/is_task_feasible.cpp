// Copyright 2023 Gustavo Rezende Silva
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include "suave_bt/is_task_feasible.hpp"

using namespace std::chrono_literals;

IsTaskFeasible::IsTaskFeasible(
  const std::string & xml_tag_name,
  const BT::NodeConfig & conf)
: BT::ConditionNode(xml_tag_name, conf)
{
  node_ = config().blackboard->get<rclcpp::Node::SharedPtr>("node");

  selectable_tasks_client =
    node_->create_client<metacontrol_kb_msgs::srv::TasksMatched>("/metacontrol_kb/task/selectable");
}

BT::NodeStatus IsTaskFeasible::tick()
{
  std::string task_name;
  getInput("task_name", task_name);

  auto request = std::make_shared<metacontrol_kb_msgs::srv::TasksMatched::Request>();

  while (!selectable_tasks_client->wait_for_service(1s)) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
      return BT::NodeStatus::FAILURE;
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
  }

  auto response = selectable_tasks_client->async_send_request(request);
  if (rclcpp::spin_until_future_complete(node_, response) ==
    rclcpp::FutureReturnCode::SUCCESS)
  {
    auto selectable_tasks = response.get()->tasks;
    for (auto task: selectable_tasks){
      if (std::strcmp(task.task_name.c_str(), task_name.c_str()) == 0){
        return BT::NodeStatus::SUCCESS;
      }
    }
    return BT::NodeStatus::FAILURE;
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service tasks selectable");
    return BT::NodeStatus::FAILURE;
  }
}
