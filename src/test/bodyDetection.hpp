#ifndef BODYDETECTION_HPP
#define BODYDETECTION_HPP

#include <opencv/cv.h>
 
// Manages message with oroclient
#include <string>

std::string face_cascade_name = "/home/dumont/catkin_ws/src/face_track_visp/src/haarcascade_frontalface.xml";
std::string upperBody_cascade_name = "/home/dumont/catkin_ws/src/face_track_visp/src/haarcascade_upperbody.xml";
std::string eye_cascade_name = "/home/dumont/catkin_ws/src/face_track_visp/src/haarcascade_eye.xml";


cv::CascadeClassifier face_cascade;
cv::CascadeClassifier upperBody_cascade;
cv::CascadeClassifier eye_cascade;

CvMemStorage* storage;

// surf
std::vector<cv::Rect> gRois;

// Optical flow
cv::Mat gPrevFrame;
cv::Mat gPrevGray;
std::vector<cv::Point2f> points[2];
cv::Point2f point;
bool addRemovePt = false;

std::string window_name = "Capture - Face detection";


void trackDetectionSurf( cv::Mat frame, std::vector<cv::Rect> rois);

void trackDetectionOpticalFlow(cv::Mat frame);

void detectBodyAndFaceAndDisplay( cv::Mat frame );

void detectFaceAndDisplay( cv::Mat frame );

void detectBodyAndDisplay( cv::Mat frame );

void detectFaceAndEyeAndDisplay( cv::Mat frame );

#endif
