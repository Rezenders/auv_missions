#include "include/home_node.hpp"

BT::NodeStatus HomeAction::tick()
{
  std::cout << "Action: " << this->name() << std::endl;
  return BT::NodeStatus::SUCCESS;
}
