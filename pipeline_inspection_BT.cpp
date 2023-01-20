#include "behaviortree_cpp/bt_factory.h"
#include <iostream>

class SearchPipeline : public BT::SyncActionNode{
public:
  SearchPipeline(const std::string& name) :
      BT::SyncActionNode(name, {})
  {}

  BT::NodeStatus tick() override
  {
    std::cout << "Action: " << this->name() << std::endl;
    return BT::NodeStatus::SUCCESS;
  }
};

BT::NodeStatus PipelineFound()
{
  std::cout << "Condition pipeline found: True" << std::endl;
  return BT::NodeStatus::SUCCESS;
}

class InspectPipeline : public BT::SyncActionNode{
public:
  InspectPipeline(const std::string& name) :
      BT::SyncActionNode(name, {})
  {}

  BT::NodeStatus tick() override
  {
    std::cout << "Action: " << this->name() << std::endl;
    return BT::NodeStatus::SUCCESS;
  }
};

static const char* xml_text = R"(
 <root BTCPP_format="4" >
     <BehaviorTree ID="MainTree">
        <Sequence name="root">
            <SearchPipeline name="search_pipeline" />
            <PipelineFound name="pipeline_found" />
	    <InspectPipeline name="inspect_pipeline" />
        </Sequence>
     </BehaviorTree>
 </root>
 )";

int main(){
	std::cout<<"Pipeline inspection mission starting"<<std::endl;
	BT::BehaviorTreeFactory factory;
	factory.registerNodeType<SearchPipeline>("SearchPipeline");
	factory.registerNodeType<InspectPipeline>("InspectPipeline");
	factory.registerSimpleCondition("PipelineFound", std::bind(PipelineFound));
	
	auto tree = factory.createTreeFromText(xml_text);
	tree.tickWhileRunning();
	
	return 0;
}
