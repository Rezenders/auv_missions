#include "behaviortree_cpp/behavior_tree.h"

class HomeAction : public BT::SyncActionNode{
public:
  HomeAction(const std::string& name) :
      BT::SyncActionNode(name, {})
  {}

  BT::NodeStatus tick() override;
};
