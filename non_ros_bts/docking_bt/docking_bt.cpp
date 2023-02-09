#include "behaviortree_cpp/behavior_tree.h"
#include "behaviortree_cpp/bt_factory.h"
#include "nodes/include/detect_marker_node.hpp"
#include "nodes/include/dock_node.hpp"
#include "nodes/include/home_node.hpp"

int main(){
  BT::BehaviorTreeFactory factory;
  factory.registerNodeType<DetectMarkerAction>("DetectMarker");
	factory.registerNodeType<DockAction>("Dock");
  factory.registerNodeType<HomeAction>("Home");

  factory.registerBehaviorTreeFromFile("../docking_bt/bts/docking.xml");
  auto tree = factory.createTree("MainTree");

  std::cout<<"Start docking procedure"<<std::endl;
  tree.tickWhileRunning();
  return 0;
}
