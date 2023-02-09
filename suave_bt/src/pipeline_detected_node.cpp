#include "suave_bt/pipeline_detected_node.hpp"

PipelineDetectedNode::PipelineDetectedNode(const std::string & condition_name, const BT::NodeConfig & conf):
BT::ConditionNode(condition_name, conf), sub_topic_("pipeline/detected")
{
  ros_node_ = conf.blackboard->get<rclcpp_lifecycle::LifecycleNode::SharedPtr>("node");

    // Setup subscription on running this node
    // Subscription will run when lifecycle node executor is called
  pipeline_detection_sub_ = ros_node_->create_subscription<std_msgs::msg::Bool>(
    sub_topic_,
    rclcpp::SystemDefaultsQoS(),
    std::bind(&PipelineDetectedNode::pipeline_detected_cb, this, std::placeholders::_1));

    // bool force_pong;
    // getInput("force_pong", force_pong);

    // if(force_pong)
    // {
    //     // assume we're transmitting the first pong
    //     pong_id_received_=0;
    // }
  return;
}

PipelineDetectedNode::~PipelineDetectedNode()
{
  RCLCPP_INFO(ros_node_->get_logger(), "SHUTTING DOWN PONG RECEIVED NODE");
  pipeline_detection_sub_.reset();
}

BT::NodeStatus PipelineDetectedNode::tick()
{
  return (pipeline_detected_ == true) ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

  RCLCPP_INFO(ros_node_->get_logger(), "Tick pipeline detected condition");

  // record last pong id received so that other nodes could use it
  // setOutput("last_pong_id", pong_id_received_);
}

void PipelineDetectedNode::pipeline_detected_cb(std_msgs::msg::Bool::SharedPtr msg)
{
  RCLCPP_INFO(ros_node_->get_logger(), "Pipeline detected: %d", msg->data);
  pipeline_detected_ = msg->data;
  return;
}
