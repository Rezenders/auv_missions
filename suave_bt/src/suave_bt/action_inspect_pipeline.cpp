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

#include "suave_bt/action_inspect_pipeline.hpp"

namespace suave_bt
{
  using namespace std::placeholders;

  InspectPipeline::InspectPipeline(
    const std::string& name, const BT::NodeConfig & conf)
  : metacontrol_plan::MetacontroledAction(name, conf), _pipeline_inspected(false)
  {
    pipeline_inspected_sub_  = node_->create_subscription<std_msgs::msg::Bool>(
      "/pipeline/inspected",
      10,
      std::bind(&InspectPipeline::pipeline_inspected_cb, this, _1));
  }

  void
  InspectPipeline::pipeline_inspected_cb(const std_msgs::msg::Bool &msg)
  {
    _pipeline_inspected = msg.data;
  }

  BT::NodeStatus InspectPipeline::onStart()
  {
    return metacontrol_plan::MetacontroledAction::onStart();
  }

  void InspectPipeline::onHalted(){
    metacontrol_plan::MetacontroledAction::onHalted();
  }

  BT::NodeStatus InspectPipeline::onRunning()
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    if(_pipeline_inspected==true){
      std::cout << "Async action finished: "<< this->name() << std::endl;
      return BT::NodeStatus::SUCCESS;
    }
    std::cout<<"Inspecting pipeline! "<<std::endl;
    return BT::NodeStatus::RUNNING;
  }

} //namespace suave_bt
