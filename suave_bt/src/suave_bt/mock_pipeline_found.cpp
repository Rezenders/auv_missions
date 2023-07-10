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

#include "suave_bt/mock_pipeline_found.hpp"

BT::NodeStatus
MockPipelineFound::pipeline_found()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  ++count;
  // RCLCPP_INFO(node_->get_logger(), "Tick pipeline found condition");
  return (count>25) ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
}