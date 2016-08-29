#include "detectionTracking.hpp"

#include <iostream>
#include <ctype.h>
#include <stdio.h>

#include <opencv/cv.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/legacy/legacy.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

#include "opencv2/video/tracking.hpp"

#include "mainPF.hpp"


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
  
  //equalizeHist(faceROI, faceROI);

  //-- Detect faces
  face_cascade.detectMultiScale( faceROI, faces, 1.3, 4, 0, Size(40, 40) );
  
  if(faces.size() > 0)
  {
    if((nbFrame == RESET_FRAME_NUMBER))
    {
      initMainParticleFilter(frame, faces[0]);
      nbFrame=0;
    }
    
    nbFrame++;
    
    int i;
    i = mainParticleFilter(frame);
  }
  
  /*if(faces.size() == nbRoi)
  {
  
  } else
  {
    // For each point, check position and confidence
    
    // If confidence is low : remove
    
    // if confidence is high and position pseudo equal : do nothing
    
  }*/
  
  for( size_t i = 0; i < faces.size(); i++ )
  {
    cv::rectangle(frame, cvPoint(faces[i].x, faces[i].y), cvPoint(faces[i].x + faces[i].width, faces[i].y + faces[i].height), CV_RGB(255, 0, 0), 4, 8, 0);
  }
  
  //-- Show what you got
  imshow( window_name, frame );
}

void detectFaceAndEyeAndDisplay( cv::Mat frame )
{
  std::vector<Rect> faces;
  Mat faceROI;
  cvtColor( frame, faceROI, CV_BGR2GRAY );
  
  //equalizeHist(faceROI, faceROI);

  //-- Detect faces
  face_cascade.detectMultiScale( faceROI, faces, 1.3, 4, 0, Size(40, 40) );

  for( size_t i = 0; i < faces.size(); i++ )
  {
    cv::rectangle(frame, cvPoint(faces[i].x, faces[i].y), cvPoint(faces[i].x + faces[i].width, faces[i].y + faces[i].height), CV_RGB(255, 0, 0), 4, 8, 0);
  
    // --- Iterate over all of the faces -> find eyes to improve detection

    // Find center of faces
    Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);
    
    Mat face = faceROI(faces[i]);
    std::vector<Rect> eyes;
    
    // Try to detect eyes, inside each face
    eye_cascade.detectMultiScale(frame, eyes, 1.1, 1, 0, Size(20, 20) );

    // Check to see if eyes inside of face, if so, draw ellipse around face
    if(eyes.size() > 0)
    {
      ellipse(frame, center, Size(faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
      printf("titi\n");
    }
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
