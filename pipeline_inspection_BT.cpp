#include "behaviortree_cpp/behavior_tree.h"
#include "behaviortree_cpp/bt_factory.h"

class SearchPipeline : public BT::StatefulActionNode{
  
  private:
    std::chrono::system_clock::time_point _completion_time;
  
  public:
  SearchPipeline(const std::string& name) :
      BT::StatefulActionNode(name, {})
  {}

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
    std::cout<<"Still searching for pipeline"<<std::endl;
    return BT::NodeStatus::RUNNING;
  }
  
  void onHalted() override{
    std::cout<< "Async action halted: "<< this->name() <<std::endl;
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
        <ReactiveSequence name="root">
          <SearchPipeline name="search_pipeline" />
          <PipelineFound name="pipeline_found" />
	        <InspectPipeline name="inspect_pipeline" />
        </ReactiveSequence>
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
 
  auto status = BT::NodeStatus::RUNNING;
  while(status == BT::NodeStatus::RUNNING) 
  {
    tree.sleep(std::chrono::milliseconds(500));

    std::cout << "--- ticking\n";
    status = tree.tickOnce();
    std::cout << "--- status: " << BT::toStr(status) << "\n\n";
  }	
	return 0;
}
