#include "behaviortree_cpp/behavior_tree.h"

class DetectMarkerAction : public BT::SyncActionNode{
public:
  DetectMarkerAction(const std::string& name) :
      BT::SyncActionNode(name, {})
  {}

  BT::NodeStatus tick() override;
};
