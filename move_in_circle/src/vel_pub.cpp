#include <stdlib.h>
#include <vector>
#include <iostream>

#include "ros/ros.h"

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Int8MultiArray.h"

#include "std_msgs/Empty.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"

#include <stdio.h>
#include <cmath>

using namespace std;

ros::Publisher pub;
int a=0,b=0;

bool flag = true;
float areaInit=0,area=0,yinit=0;
float Arr[4];
int erra=0,erry=0;

geometry_msgs::Twist vel;

void velPublisher(const std_msgs::Int32MultiArray::ConstPtr& array)
{
	int i=0;
	for(std::vector<int>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
	{
		Arr[i] = *it;
		i++;
	}
	if(flag)
    {
    	areaInit = Arr[2]*Arr[3];
    	flag = false;
    	yinit=Arr[0];
    }
    
    area=Arr[2]*Arr[3];
	vel.linear.z=0;
	vel.angular.z=0;
	if(abs(areaInit-area)>80)
    {//pd

    	vel.linear.x = 0.00004*(areaInit-area)+ 0.00005*((areaInit-area)-erra);
    }
    if (abs(areaInit-area)<=80)
    {
    	vel.linear.x=0.00;
    }
    if(abs(yinit-Arr[0])>10)
    {//pd
    	vel.linear.y=0.0006*(yinit-Arr[0])+0.001*((yinit-Arr[0])-erry) ;
    }
    else
    	vel.linear.y=0;
    erry=yinit-Arr[0];
    erra=areaInit-area;
    pub.publish(vel);
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "vel_pub");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe<std_msgs::Int32MultiArray>("/logo_Info",100,velPublisher);
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