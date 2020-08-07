#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
#include <math.h>

double pick_X = 2.0;
double pick_Y = 2.0;
double drop_X = -2.0;
double drop_Y = 2.0;

bool item_pick = false;
bool item_drop = false;

void odometry_call(const nav_msgs::Odometry::ConstPtr& msg) {
  double x_robot = msg->pose.pose.position.x;
  double y_robot = msg->pose.pose.position.y;
  
  double dist_pick = sqrt(pow(x_robot - pick_X, 2) + pow(y_robot - pick_Y, 2));
  double dist_drop = sqrt(pow(x_robot - drop_X, 2) + pow(y_robot - drop_Y, 2));  

  if (dist_pick < 0.3) {
    item_pick = true;
  }
  
  if (dist_drop < 0.3) {
    item_drop = true;
  }
}


int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber odometry_sub = n.subscribe("/odom", 1000, odometry_call);

  visualization_msgs::Marker marker;

 
  uint32_t shape = visualization_msgs::Marker::CUBE;


  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();


  marker.ns = "cube";
  marker.id = 0;

 
  marker.type = shape;


  marker.pose.position.x = pick_X;
  marker.pose.position.y = pick_Y;
  marker.pose.position.z = 0;
  marker.pose.orientation.x = 0.0;
  marker.pose.orientation.y = 0.0;
  marker.pose.orientation.z = 0.0;
  marker.pose.orientation.w = 1.0;

  // Set the scale of the marker -- 1x1x1 here means 1m on a side
  marker.scale.x = 0.5;
  marker.scale.y = 0.5;
  marker.scale.z = 0.5;

  // Set the color -- be sure to set alpha to something non-zero!
  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;

  marker.lifetime = ros::Duration();
  while (ros::ok())
  {
    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    
    if (!item_pick)
    {
      marker.action = visualization_msgs::Marker::ADD;
      marker_pub.publish(marker);
    }
    else if (!item_drop)
    {
      marker.action = visualization_msgs::Marker::DELETE;
      marker_pub.publish(marker);
    }
    else
    {
      marker.pose.position.x = drop_X;
      marker.pose.position.y = drop_Y;
      marker.action = visualization_msgs::Marker::ADD;
      marker_pub.publish(marker);
    }
    
    ros::spinOnce();
  }
}
