#ifndef BODYDETECTION_HPP
#define BODYDETECTION_HPP

#include <opencv/cv.h>
 
// Manages message with oroclient
#include <string>

std::string face_cascade_name = "/home/dumont/catkin_ws/src/face_track_visp/haarcascade/haarcascade_frontalface.xml";
std::string upperBody_cascade_name = "/home/dumont/catkin_ws/src/face_track_visp/haarcascade/haarcascade_upperbody.xml";
std::string eye_cascade_name = "/home/dumont/catkin_ws/src/face_track_visp/haarcascade/haarcascade_eye.xml";


cv::CascadeClassifier face_cascade;
cv::CascadeClassifier upperBody_cascade;
cv::CascadeClassifier eye_cascade;

//size_t nbRoi;
int nbFrame;

CvMemStorage* storage;

std::string window_name = "Capture - Face detection";

#define RESET_FRAME_NUMBER 100


void trackParticlFlow(cv::Mat frame);

void detectBodyAndFaceAndDisplay( cv::Mat frame );

void detectFaceAndDisplay( cv::Mat frame );

void detectBodyAndDisplay( cv::Mat frame );

void detectFaceAndEyeAndDisplay( cv::Mat frame );

#endif
