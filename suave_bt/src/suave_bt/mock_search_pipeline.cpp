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

#include "suave_bt/mock_search_pipeline.hpp"

namespace suave_bt
{
  SearchPipeline::SearchPipeline(
    const std::string& name, const BT::NodeConfig & conf)
  : BT::StatefulActionNode(name, conf)
  {
    node_ = config().blackboard->get<rclcpp::Node::SharedPtr>("node");

    pipeline_detection_pub_  = node_->create_publisher<std_msgs::msg::Bool>(
      "/pipeline/detected", 10);
  }

  BT::NodeStatus SearchPipeline::onStart(){
    std::cout << "Async action starting: " << this->name() << std::endl;
    _completion_time = std::chrono::system_clock::now() + std::chrono::milliseconds(5000);
    return BT::NodeStatus::RUNNING;
  }

  BT::NodeStatus SearchPipeline::onRunning(){
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    if(std::chrono::system_clock::now() >= _completion_time){
      std::cout << "Async action finished: "<< this->name() << std::endl;
      std_msgs::msg::Bool msg;
      msg.data = true;
      pipeline_detection_pub_->publish(msg);
      return BT::NodeStatus::SUCCESS;
    }
    std::cout<<"Searching for pipeline! "<<std::endl;
    return BT::NodeStatus::RUNNING;
  }
} //namespace suave_bt
