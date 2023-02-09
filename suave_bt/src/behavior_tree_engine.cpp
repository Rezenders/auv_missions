#include <rclcpp/rclcpp.hpp>
// #include <rclcpp/executors.hpp>
#include "behaviortree_cpp/behavior_tree.h"
#include "behaviortree_cpp/action_node.h"
#include "behaviortree_cpp/condition_node.h"
#include "behaviortree_cpp/bt_factory.h"
#include "suave_bt/pipeline_detected_node.hpp"


//-------------------------------------------------------------
// Simple Action to print a number
//-------------------------------------------------------------

class PrintValue : public BT::SyncActionNode
{
public:
  PrintValue(const std::string& name, const BT::NodeConfig& config)
  : BT::SyncActionNode(name, config) {}

  BT::NodeStatus tick() override {
    std::string msg;
    if( getInput("message", msg ) ){
      std::cout << "PrintValue: " << msg << std::endl;
      return BT::NodeStatus::SUCCESS;
    }
    else{
      std::cout << "PrintValue FAILED "<< std::endl;
      return BT::NodeStatus::FAILURE;
    }
  }

  static BT::PortsList providedPorts() {
    return{ BT::InputPort<std::string>("message") };
  }
};


// Simple tree, used to execute once each action.
static const char* xml_text = R"(
<root >
   <BehaviorTree>
      <Sequence>
          <PrintValue message="start"/>
          <Sleep msec="2000"/>
          <PrintValue message="sleep completed"/>
          <Fallback>
              <Timeout msec="500">
                 <Sleep msec="1000"/>
              </Timeout>
              <PrintValue message="sleep aborted"/>
          </Fallback>
      </Sequence>
   </BehaviorTree>
</root>
)";

int main(int argc, char **argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("bt_node");

  BT::BehaviorTreeFactory factory;
  factory.registerNodeType<PrintValue>("PrintValue");

  // auto tree = factory.createTreeFromText(xml_text);

  BT::NodeStatus status = BT::NodeStatus::IDLE;

  // while( rclcpp::ok() )
  // {
  //   status = tree.tickRoot();
  //   tree.sleep(std::chrono::milliseconds(100));
  // }

  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
