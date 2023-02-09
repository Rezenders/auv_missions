#include "include/detect_marker_node.hpp"

BT::NodeStatus DetectMarkerAction::tick()
{
  std::cout << "Action: " << this->name() << std::endl;
  return BT::NodeStatus::SUCCESS;
}
