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

#include "suave_bt/mock_enough_battery.hpp"
#include "suave_bt/mock_pipeline_found.hpp"

namespace suave_bt
{

using namespace std::placeholders;

MockEnoughBattery::MockEnoughBattery(
  const std::string & xml_tag_name,
  const BT::NodeConfig & conf)
: BT::ConditionNode(xml_tag_name, conf), _battery_charged(true), count(0)
{
  node_ = config().blackboard->get<rclcpp::Node::SharedPtr>("node");

  battery_charged_sub_  = node_->create_subscription<std_msgs::msg::Bool>(
    "/battery/charged",
    10,
    std::bind(&MockEnoughBattery::battery_charged_cb, this, _1));
}

void
MockEnoughBattery::battery_charged_cb(const std_msgs::msg::Bool &msg)
{
  _battery_charged = msg.data;
  if(_battery_charged == true){
    count = 0;
  }
}

BT::NodeStatus MockEnoughBattery::tick()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  ++count;
  if (count>2000){
    _battery_charged = false;
  }
  return (_battery_charged==true) ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
}

}  // namespace suave_bt
