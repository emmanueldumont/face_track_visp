#include "bodyDetection.hpp"

#include <iostream>

#include <opencv/cv.h>

#include "opencv2/objdetect/objdetect.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;

void detectBodyAndFaceAndDisplay( cv::Mat frame )
{
 std::vector<Rect> upperBody;
 Mat frame_gray;
 cvtColor( frame, frame_gray, CV_BGR2GRAY );
 equalizeHist( frame_gray, frame_gray );

 //-- Detect faces
 upperBody_cascade.detectMultiScale( frame_gray, upperBody, 1.1, 4, 0, Size(80, 80) );
 for( size_t i = 0; i < upperBody.size(); i++ )
 {
   Point center( upperBody[i].x + upperBody[i].width*0.5, upperBody[i].y + upperBody[i].height*0.5 );
   ellipse( frame, center, Size( upperBody[i].width*0.5, upperBody[i].height*0.5), 0, 0, 360, Scalar( 0, 0, 255 ), 4, 8, 0 );
   Mat faceROI = frame_gray( upperBody[i] );
   std::vector<Rect> faces;

   //-- In each upperBody, detect faces
   face_cascade.detectMultiScale( faceROI, faces, 1.3, 2, 0, Size(40, 40) );

   for( size_t j = 0; j < faces.size(); j++ )
    {
      Point center( upperBody[i].x + faces[j].x + faces[j].width*0.5, upperBody[i].y + faces[j].y + faces[j].height*0.5 );
      int radius = cvRound( (faces[j].width + faces[j].height)*0.25 );
      circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
    }
 }
 //-- Show what you got
 imshow( window_name, frame );
}



void detectFaceAndDisplay( cv::Mat frame )
{
  std::vector<Rect> faces;
  Mat faceROI;
  cvtColor( frame, faceROI, CV_BGR2GRAY );

  //-- Detect faces
  face_cascade.detectMultiScale( faceROI, faces, 1.3, 4, 0, Size(40, 40) );

  for( size_t i = 0; i < faces.size(); i++ )
  {
    cv::rectangle(frame, cvPoint(faces[i].x, faces[i].y), cvPoint(faces[i].x + faces[i].width, faces[i].y + faces[i].height), CV_RGB(255, 0, 0), 4, 8, 0);
  }
  //-- Show what you got
  imshow( window_name, frame );
}



void detectBodyAndDisplay( cv::Mat frame )
{
  std::vector<Rect> upperBody;
  Mat frame_gray;
  
  cvtColor( frame, frame_gray, CV_BGR2GRAY );

  //-- Detect body
  upperBody_cascade.detectMultiScale( frame_gray, upperBody, 1.1, 4, 0, Size(80, 80) );
  
  for( size_t i = 0; i < upperBody.size(); i++ )
  {
    cv::rectangle(frame, cvPoint(upperBody[i].x, upperBody[i].y), cvPoint(upperBody[i].x + upperBody[i].width, upperBody[i].y + upperBody[i].height), CV_RGB(0, 0, 255), 4, 8, 0);
  }
  //-- Show what you got
  imshow( window_name, frame );
}
