#include "behaviortree_cpp/behavior_tree.h"
#include "behaviortree_cpp/bt_factory.h"

class SearchPipeline : public BT::StatefulActionNode{

  protected:
    // std::chrono::system_clock::time_point _completion_time;
    float altitude;

  public:
  SearchPipeline(const std::string& name) :
      BT::StatefulActionNode(name, {}), altitude(0.0) {}

  BT::NodeStatus onStart() override{
    std::cout << "Async action starting: " << this->name() << std::endl;
    // _completion_time = std::chrono::system_clock::now() + std::chrono::milliseconds(5000);
    return BT::NodeStatus::RUNNING;
  }

  BT::NodeStatus onRunning() override{
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // if(std::chrono::system_clock::now() >= _completion_time){
    //   std::cout << "Async action finished: "<< this->name() << std::endl;
    //   return BT::NodeStatus::SUCCESS;
    // }
    std::cout<<"Searching for pipeline at altitude: "<<altitude<<std::endl;
    return BT::NodeStatus::RUNNING;
  }

  void onHalted() override{
    std::cout<< "Async action halted: "<< this->name() <<std::endl;
  }

};

class SearchPipelineLow : public SearchPipeline{
public:
  SearchPipelineLow(const std::string& name) :
      SearchPipeline(name) {altitude = 1.0;}
};

class SearchPipelineHigh : public SearchPipeline{
public:
  SearchPipelineHigh(const std::string& name) :
      SearchPipeline(name) {altitude = 3.0;}
};

class PipelineDetection{
private:
  int count;
public:
  PipelineDetection(): count(10) {}
  BT::NodeStatus pipeline_found(){
    if(count>50){
      std::cout << "Condition pipeline found: True" << std::endl;
      return BT::NodeStatus::SUCCESS;
    }else{
      ++count;
      return BT::NodeStatus::FAILURE;
    }
  }
};

class WaterVisibility{
private:
  double water_visibility;
  double delta;
public:
  WaterVisibility() : water_visibility(1.5), delta(-0.1) {}

  void update_water_visibility(){
    std::cout<<"Current water visibility "<<water_visibility<<std::endl;
    if (water_visibility <= 0.1){
      delta = +0.1;
    }else if (water_visibility > 3.5){
      delta = -0.1;
    }
    water_visibility += delta;
  }

  BT::NodeStatus is_water_visibility_low(){
    update_water_visibility();
    return (water_visibility <= 1.0) ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
  }
  BT::NodeStatus is_water_visibility_high(){
    update_water_visibility();
    return (water_visibility > 1.0) ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
  }
};

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

int main(){
	std::cout<<"Pipeline inspection mission starting"<<std::endl;
	BT::BehaviorTreeFactory factory;
	// factory.registerNodeType<SearchPipeline>("SearchPipeline");
	factory.registerNodeType<SearchPipelineLow>("SearchLow");
	factory.registerNodeType<SearchPipelineHigh>("SearchHigh");
	factory.registerNodeType<InspectPipeline>("InspectPipeline");

  PipelineDetection pipeline_detection;
	factory.registerSimpleCondition("PipelineFound",
    std::bind(&PipelineDetection::pipeline_found, pipeline_detection));

  WaterVisibility water_visibility;
  factory.registerSimpleCondition("WaterVisibilityLow",
    std::bind(&WaterVisibility::is_water_visibility_low, &water_visibility));
  factory.registerSimpleCondition("WaterVisibilityHigh",
    std::bind(&WaterVisibility::is_water_visibility_high, &water_visibility));

	// auto tree = factory.createTreeFromText(xml_text);
	// auto tree = factory.createTreeFromFile("../pipeline_inspection_groot.xml");
	factory.registerBehaviorTreeFromFile("../pipeline_inspection_groot.xml");
  auto tree = factory.createTree("MainTree");

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
