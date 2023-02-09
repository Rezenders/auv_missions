#ifndef PIPELINE_DETECTED_NODE_H_
#define PIPELINE_DETECTED_NODE_H_

#include "behaviortree_cpp/behavior_tree.h"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "std_msgs/msg/bool.hpp"

/// @brief Condition Node that indicates if the pipeline was detected
class PipelineDetectedNode : public BT::ConditionNode
{
public:
  /// @brief PipelineDetectedNode constructor
  /// @param condition_name Name for the XML tag for this node
  /// @param conf BT Node Configuration
  PipelineDetectedNode(const std::string & condition_name, const BT::NodeConfig & conf);
  PipelineDetectedNode() = delete;
  ~PipelineDetectedNode();

  ///@brief Creates list of BT ports
  ///@return BT::PortsList Containing node-specific ports
  // static BT::PortsList providedPorts()
  // {
  //    return {
  //        BT::InputPort<bool>("force_pong"),
  //        BT::OutputPort<int32_t>("last_pong_id"),
  //    };
  // }

  /// @brief The main behavior call when this node is run
  /// @return NodeStatus Success or Failiure
  BT::NodeStatus tick() override;

   /// @brief Record the last received pong
  void pipeline_detected_cb(std_msgs::msg::Bool::SharedPtr msg);

private:

   /// @brief Subscription to the pong message from a tertiary node
   std::string sub_topic_;
   // int32_t pong_id_received_;
   bool pipeline_detected_ = false;
   rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr pipeline_detection_sub_;
   rclcpp_lifecycle::LifecycleNode::SharedPtr ros_node_;
};

#endif
