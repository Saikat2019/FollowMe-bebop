# FollowMe-bebop
#ROS# #COMPUTER_VISION#

Make a package in bebop workspace and add these files over there to run them.

around.cpp     -  make the parrot drone revolve in circular motion without any yaw and without pid tuning.

camViewMat.cpp -  track a particular area you select by takin input from bebop camera.

circle.cpp     -  make bebop rotate at same point by taking input from odom and tuning p parameter.

line.cpp       -  make bebop go in a straight line by taking input from odom and tuning p parameters.

vel_pub.cpp    -  see the tracking area and move parrot accordingly.

yaw_mot.cpp    -  make bebop rotate in circular motion by also giving its yaw.

All of these are ros commands and can be operated by rosrun.
