#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv) {
  ros::init(argc, argv, "pick_objects");
  
  MoveBaseClient ac("move_base", true);
  
  while(!ac.waitForServer(ros::Duration(5.0))) {
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  
  move_base_msgs::MoveBaseGoal goal;
  
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  goal.target_pose.pose.position.x = 2.0;
  goal.target_pose.pose.position.y = 2.0;
  goal.target_pose.pose.orientation.w = 1.0;
  
  ROS_INFO("Sending goal");
  ac.sendGoal(goal);
  
  ac.waitForResult();
  
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Super the goal,is reached by the robot!");
  else
    ROS_INFO("The robot failed to initiate");
    
  ros::Duration(5.0).sleep();

  goal.target_pose.pose.position.x = -2.0;
  goal.target_pose.pose.position.y = 2.0;
  goal.target_pose.pose.orientation.w = 1.0;
  
  ROS_INFO("Sending goal");
  ac.sendGoal(goal);
  
  ac.waitForResult();
  
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Fantastic the robot returned back to initial start!");
  else
    ROS_INFO("The robot failed to move.");
    
  ros::Duration(5.0).sleep();
  
  return 0;
}
