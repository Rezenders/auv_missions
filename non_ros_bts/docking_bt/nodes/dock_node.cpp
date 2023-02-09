#include "include/dock_node.hpp"

BT::NodeStatus DockAction::tick()
{
  std::cout << "Action: " << this->name() << std::endl;
  return BT::NodeStatus::SUCCESS;
}
