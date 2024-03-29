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

#ifndef SUAVE_BT__MOCK_ENOUGH_BATTERY_HPP_
#define SUAVE_BT__MOCK_ENOUGH_BATTERY_HPP_

#include "behaviortree_cpp/behavior_tree.h"
#include "behaviortree_cpp/bt_factory.h"

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"

namespace suave_bt
{

class IsEnoughBattery : public BT::ConditionNode
{
public:
  IsEnoughBattery(const std::string & xml_tag_name,
    const BT::NodeConfig & conf);

  BT::NodeStatus tick() override;

  static BT::PortsList providedPorts()
  {
    return BT::PortsList(
      {
      });
  }

private:
  std::chrono::system_clock::time_point _expected_discharge;
  bool _battery_charged;

  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr battery_charged_sub_;

  void battery_charged_cb(const std_msgs::msg::Bool &msg);
};

}  // namespace suave_bt

#endif  // SUAVE_BT__MOCK_ENOUGH_BATTERY_HPP_
