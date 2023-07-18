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

#include "suave_bt/mock_inspect_pipeline.hpp"

namespace suave_bt
{
  InspectPipeline::InspectPipeline(
    const std::string& name, const BT::NodeConfig & conf)
  : MetacontroledAction(name, conf), _initial_inspection(true)
  {
    pipeline_inspection_pub_  = node_->create_publisher<std_msgs::msg::Bool>(
      "/pipeline/inspected", 10);
  }

  BT::NodeStatus InspectPipeline::onStart()
  {
    if(_initial_inspection == true){
      _completion_time = std::chrono::system_clock::now() + std::chrono::milliseconds(20000);
      _initial_inspection = false;
    } else{
      _completion_time = std::chrono::system_clock::now() + _missing_time;
    }
    return MetacontroledAction::onStart();
  }

  void InspectPipeline::onHalted(){
    _missing_time = std::chrono::duration_cast<std::chrono::milliseconds>(_completion_time - std::chrono::system_clock::now());
    MetacontroledAction::onHalted();
  }

  BT::NodeStatus InspectPipeline::onRunning()
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    if(std::chrono::system_clock::now() >= _completion_time){
      std::cout << "Async action finished: "<< this->name() << std::endl;
      std_msgs::msg::Bool msg;
      msg.data = true;
      pipeline_inspection_pub_->publish(msg);
      return BT::NodeStatus::SUCCESS;
    }
    std::cout<<"Inspecting pipeline! "<<std::endl;
    return BT::NodeStatus::RUNNING;
  }

} //namespace suave_bt
