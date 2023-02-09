#include "behaviortree_cpp/behavior_tree.h"

class DockAction : public BT::SyncActionNode{
public:
  DockAction(const std::string& name) :
      BT::SyncActionNode(name, {})
  {}

  BT::NodeStatus tick() override;
};
