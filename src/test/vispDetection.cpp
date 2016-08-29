#include <iostream>

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/highgui/highgui.hpp>

#include <opencv/cv.h>
#include <stdio.h>

#include <signal.h>         // Handling ctrl+C signal to close everything properly

// Manages message with oroclient
#include "std_msgs/String.h"
#include <sstream>

#include "opencv2/objdetect/objdetect.hpp"
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/nonfree/nonfree.hpp>

#include "bodyDetection.hpp"

using namespace std;
using namespace cv;

static const uint32_t MY_ROS_QUEUE_SIZE = 1000;


// Create a function which manages a clean "CTRL+C" command -> sigint command
void sigint_handler(int dummy)
{
    ROS_INFO("- movector is shutting down...\n");
    
    // Liberation de l'espace memoire
    cv::destroyWindow(window_name);
    cv::destroyWindow("track");
    cv::destroyWindow("Matched");

    ROS_INFO("\n\n... Bye bye !\n\t-Manu\n");
    exit(EXIT_SUCCESS); // Shut down the program
}

void imgcb(const sensor_msgs::Image::ConstPtr& msg)
{
  // Convert ROS images to cv::mat
  cv_bridge::CvImageConstPtr cv_ptr;
  cv_ptr = cv_bridge::toCvShare(msg);
  IplImage *img = new IplImage(cv_ptr->image); 

  //cv::imshow("Initial", cv_ptr->image);
  cv::waitKey(1);  // Update screen
  
  // Processing
  try
  {
    detectFaceAndDisplay( cv_ptr->image );
    
  } catch (const cv_bridge::Exception& e) {
      ROS_ERROR("cv_bridge exception: %s", e.what());
  }
}



int main(int argc, char* argv[])
{
  ros::init(argc, argv, "movector");

  std::cout << "Oh hai there!" << std::endl;

  ros::NodeHandle nh;
  
  initModule_nonfree();  
  
  // Override the signal interrupt from ROS
  signal(SIGINT, sigint_handler);
  
  if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading frontal\n"); return -1; };
  if( !upperBody_cascade.load( upperBody_cascade_name ) ){ printf("--(!)Error loading upper\n"); return -1; };
  if( !upperBody_cascade.load( eye_cascade_name ) ){ printf("--(!)Error loading eye\n"); return -1; };
  
  storage = cvCreateMemStorage(0);
    
  // ros::Subscriber sub = nh.subscribe("camera/rgb/image_raw", MY_ROS_QUEUE_SIZE, imgcb);
  ros::Subscriber sub = nh.subscribe("camera/rgb/image_color", MY_ROS_QUEUE_SIZE, imgcb);

  ros::spin();

  std::cout << "byebye my friend" << std::endl;

  return 0;
}

