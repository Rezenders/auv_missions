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

#include "suave_bt/is_enough_battery.hpp"
// #include "suave_bt/is_pipeline_found.hpp"

namespace suave_bt
{

using namespace std::placeholders;

IsEnoughBattery::IsEnoughBattery(
  const std::string & xml_tag_name,
  const BT::NodeConfig & conf)
: BT::ConditionNode(xml_tag_name, conf), _battery_charged(true)
{
  node_ = config().blackboard->get<rclcpp::Node::SharedPtr>("node");

  battery_charged_sub_  = node_->create_subscription<std_msgs::msg::Bool>(
    "/battery/charged",
    10,
    std::bind(&IsEnoughBattery::battery_charged_cb, this, _1));

  _expected_discharge = std::chrono::system_clock::now() + std::chrono::milliseconds(10000);
}

void
IsEnoughBattery::battery_charged_cb(const std_msgs::msg::Bool &msg)
{
  _battery_charged = msg.data;
  if(_battery_charged == true){
    _expected_discharge = std::chrono::system_clock::now() + std::chrono::milliseconds(10000);
  }
}

BT::NodeStatus IsEnoughBattery::tick()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  if(std::chrono::system_clock::now() >= _expected_discharge){
   _battery_charged = false;
  }
  return (_battery_charged==true) ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
}

}  // namespace suave_bt
