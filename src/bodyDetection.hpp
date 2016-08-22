#include <opencv/cv.h>

//#include "opencv2/objdetect/objdetect.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
 
// Manages message with oroclient
#include <string>

std::string face_cascade_name = "/home/dumont/catkin_ws/src/face_track_visp/src/haarcascade_frontalface.xml";
std::string upperBody_cascade_name = "/home/dumont/catkin_ws/src/face_track_visp/src/haarcascade_upperbody.xml";


cv::CascadeClassifier face_cascade;
cv::CascadeClassifier upperBody_cascade;

std::string window_name = "Capture - Face detection";


void detectBodyAndFaceAndDisplay( cv::Mat frame );

void detectFaceAndDisplay( cv::Mat frame );

void detectBodyAndDisplay( cv::Mat frame );
