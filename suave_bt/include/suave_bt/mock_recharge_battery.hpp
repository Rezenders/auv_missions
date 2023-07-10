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

#ifndef SUAVE_BT__MOCK_RECHARGE_BATTERY_HPP_
#define SUAVE_BT__MOCK_RECHARGE_BATTERY_HPP_

#include "behaviortree_cpp/behavior_tree.h"
#include "behaviortree_cpp/bt_factory.h"

namespace suave_bt
{

class RechargeBattery : public BT::StatefulActionNode{

  protected:
    std::chrono::system_clock::time_point _completion_time;

  public:
  RechargeBattery(const std::string& name) :
      BT::StatefulActionNode(name, {}){}

  BT::NodeStatus onStart() override{
    std::cout << "Async action starting: " << this->name() << std::endl;
    _completion_time = std::chrono::system_clock::now() + std::chrono::milliseconds(5000);
    return BT::NodeStatus::RUNNING;
  }

  BT::NodeStatus onRunning() override{
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    if(std::chrono::system_clock::now() >= _completion_time){
      std::cout << "Async action finished: "<< this->name() << std::endl;
      return BT::NodeStatus::SUCCESS;
    }
    std::cout<<"Recharging battery! "<<std::endl;
    return BT::NodeStatus::RUNNING;
  }

  void onHalted() override{
    std::cout<< "Async action halted: "<< this->name() <<std::endl;
  }

};

}  // namespace suave_bt

#endif  // SUAVE_BT__MOCK_RECHARGE_BATTERY_HPP_
