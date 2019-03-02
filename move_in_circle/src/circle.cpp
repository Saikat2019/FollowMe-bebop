#include "ros/ros.h"
#include "std_msgs/Empty.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include <stdio.h>
#include <cmath>

ros::Publisher pub;
int a=0,b=0;

geometry_msgs::Twist vel;
void chatterCallBack(const nav_msgs::Odometry::ConstPtr& msg)
{
    while((abs(msg->pose.pose.position.x-a)>0.02)||(abs(msg->pose.pose.position.y-b)>0.02))
	{
		vel.linear.x=0.15*(a-msg->pose.pose.position.x);
		vel.linear.y=0.15*(b-msg->pose.pose.position.y);
	}
	vel.linear.y = 0.0;
	vel.linear.x=-0.0;
	
	vel.linear.z=0;
	vel.angular.z+=0.00064;
	a=msg->pose.pose.position.x;
	b=msg->pose.pose.position.y;
	pub.publish(vel);
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "circle");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe<nav_msgs::Odometry>("/bebop/odom",100,chatterCallBack);
  pub = n.advertise<geometry_msgs::Twist>("/bebop/cmd_vel", 100);
  ros::Rate loop_rate(10);
  int count = 0;
  vel.angular.z=0;
  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }  
  return 0;
}