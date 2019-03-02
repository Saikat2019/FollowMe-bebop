#include "ros/ros.h"

#include <iostream>
#include "cv_bridge/cv_bridge.h"
#include "sensor_msgs/Image.h"

#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <cstring>

#include <image_transport/image_transport.h>

#include "std_msgs/Empty.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"

#include <stdio.h>
#include <stdlib.h>
#include <cmath>

//#include "sensor_msgs/"
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

using namespace cv;
using namespace std;

int a=0,b=0;
int key=0;

string trackerType="MEDIANFLOW";// = trackerTypes[4];

Ptr<Tracker> tracker = TrackerMedianFlow::create();
    
Rect2d bbox(0,0, 50, 50);

ros::Publisher pub;

std_msgs::Int32MultiArray array;

void detection(Mat frame)
{
  if (key==0)
  {

    bbox = selectROI(frame, false);
    rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
    imshow("Tracking", frame);
    
    tracker->init(frame, bbox);
    key++;
  }

     
    double timer = (double)getTickCount();
        
    bool ok = tracker->update(frame, bbox);
        
        // Calculate Frames per second (FPS)
    float fps = getTickFrequency() / ((double)getTickCount() - timer);
        
        
    if (ok)
      {
        rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
      }
    else
      {
          putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
      }
        
    putText(frame, trackerType + " Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);
        
    putText(frame, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);

    imshow("Tracking", frame);

    array.data.clear();

    array.data.push_back(bbox.x);
    array.data.push_back(bbox.y);
    array.data.push_back(bbox.width);
    array.data.push_back(bbox.height);

    waitKey(1);

}
int key1=0;
void logo_info(const sensor_msgs::ImageConstPtr& msg)
{
  Mat image = cv_bridge::toCvShare(msg, "bgr8")->image;

  if (key1=1)
  {
  detection(image);
  pub.publish(array);
}
key1=1;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "circle");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe<sensor_msgs::Image>("/bebop/image_raw",100,logo_info);
  pub = n.advertise<std_msgs::Int32MultiArray>("/logo_Info", 100);
  ros::Rate loop_rate(10);
  
  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }  
  return 0;
}