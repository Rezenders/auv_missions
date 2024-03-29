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

#include "ament_index_cpp/get_package_share_directory.hpp"

#include "behaviortree_cpp/behavior_tree.h"
#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/utils/shared_library.h"
#include "behaviortree_cpp/loggers/bt_zmq_publisher.h"

#include "rclcpp/rclcpp.hpp"

#include "suave_bt/action_recharge_battery.hpp"
#include "suave_bt/action_search_pipeline.hpp"
#include "suave_bt/action_inspect_pipeline.hpp"
#include "suave_bt/is_pipeline_found.hpp"
#include "suave_bt/is_pipeline_inspected.hpp"
#include "suave_bt/is_enough_battery.hpp"
#include "suave_bt/arm_thrusters.hpp"
#include "suave_bt/set_guided_mode.hpp"
#include "rosa_plan/is_action_feasible.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("mock_suave");

  BT::BehaviorTreeFactory factory;
  BT::SharedLibrary loader;

  factory.registerNodeType<suave_bt::SearchPipeline>("search_pipeline");
  factory.registerNodeType<suave_bt::InspectPipeline>("inspect_pipeline");
  factory.registerNodeType<suave_bt::RechargeBattery>("recharge");

  factory.registerNodeType<suave_bt::IsEnoughBattery>("IsEnoughBattery");
  factory.registerNodeType<suave_bt::IsPipelineFound>("IsPipelineFound");
  factory.registerNodeType<suave_bt::IsPipelineInspected>("IsPipelineInspected");
  factory.registerNodeType<rosa_plan::IsActionFeasible>("IsActionFeasible");

  factory.registerNodeType<suave_bt::ArmThrusters>("ArmThrusters");
  factory.registerNodeType<suave_bt::SetGuidedMode>("SetGuidedMode");

  std::string pkgpath = ament_index_cpp::get_package_share_directory("suave_bt");
  std::string xml_file = pkgpath + "/bts/suave.xml";

  auto blackboard = BT::Blackboard::create();
  blackboard->set<rclcpp::Node::SharedPtr>("node", node);

  BT::Tree tree = factory.createTreeFromFile(xml_file, blackboard);

  auto publisher_zmq = std::make_shared<BT::PublisherZMQ>(tree, 10, 1666, 1667);

  rclcpp::Rate rate(10);

  bool finish = false;
  while (!finish && rclcpp::ok()) {
  // while (rclcpp::ok()) {
    finish = tree.rootNode()->executeTick() == BT::NodeStatus::SUCCESS;

    rclcpp::spin_some(node);
    rate.sleep();
  }

  rclcpp::shutdown();
  return 0;
}
