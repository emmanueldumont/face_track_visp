#include "bodyDetection.hpp"

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


using namespace std;
using namespace cv;


void drawOptFlowMap(const cv::Mat& flow, cv::Mat& cflowmap, int step, const cv::Scalar& color)
{
  namedWindow( "Matched", 1 );
  
	// Beautiful drawing for visualization purposes
    for(int y = 0; y < cflowmap.rows; y += step)
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            const cv::Point2f& fxy = flow.at<cv::Point2f>(y, x) ; // flow magnitude at that point
            line(cflowmap, cv::Point(x,y), cv::Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                 color);
            //circle(cflowmap, cv::Point(x,y), 2, color, -1);
        }
}


void trackDetectionOpticalFlow(cv::Mat frame)
{
  VideoCapture cap;
  TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
  Size subPixWinSize(10,10), winSize(31,31);

  const int MAX_COUNT = 500;

  namedWindow( "track", 1 );

  Mat gray, image;
  
  frame.copyTo(image);
  cvtColor(image, gray, COLOR_BGR2GRAY);
  
  if(!gPrevFrame.empty())
  {
    do
    {
      if( !points[0].empty() )
      {
          vector<uchar> status;
          vector<float> err;
          if(gPrevGray.empty())
              gray.copyTo(gPrevGray);
          calcOpticalFlowPyrLK(gPrevGray, gray, points[0], points[1], status, err, winSize, 3, termcrit, 0, 0.001);
          size_t i, k;
          for( i = k = 0; i < points[1].size(); i++ )
          {
              
              if( !status[i] )
                  continue;

              points[1][k++] = points[1][i];
              circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
          }
          points[1].resize(k);
      }

      if( addRemovePt && points[1].size() < (size_t)MAX_COUNT )
      {
          vector<Point2f> tmp;
          tmp.push_back(point);
          cornerSubPix( gray, tmp, winSize, Size(-1,-1), termcrit);
          points[1].push_back(tmp[0]);
          addRemovePt = false;
      }

      imshow("track", image);
    }while(0);
  }
  else
  {
    // automatic initialization
    goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 10, Mat(), 3, 0, 0.04);
    cornerSubPix(gray, points[1], subPixWinSize, Size(-1,-1), termcrit);
    addRemovePt = false;
  }
  
  std::swap(points[1], points[0]);
  cv::swap(gPrevGray, gray);
  cv::swap(gPrevFrame, frame);
}

void trackDetectionSurf( cv::Mat frame, std::vector<cv::Rect> rois)
{
  Mat outFrame;
  SurfFeatureDetector surf( 2500 );
  vector<KeyPoint> keypoints1;
  Mat descriptors1;
  SurfDescriptorExtractor surfDesc;
  
  surf.detect(frame, keypoints1);
  drawKeypoints(frame, keypoints1, outFrame, Scalar(255,255,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
  surfDesc.compute(frame, keypoints1, descriptors1);
  
  
  namedWindow("Matched");
  
  for( size_t cpt = 0; cpt < gRois.size(); cpt++ )
  {
    
    Mat outRoi;
    Mat frameROI = Mat(frame,gRois[cpt]);

    // vector of keypoints
    vector<KeyPoint> keypoints2;
    
    surf.detect(frameROI, keypoints2);
    
    drawKeypoints(frameROI, keypoints2, outRoi, Scalar(255,255,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    
    Mat descriptors2;
    surfDesc.compute(frameROI, keypoints2, descriptors2);

    BruteForceMatcher<L2<float> > matcher;
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    nth_element(matches.begin(), matches.begin()+24, matches.end());
    matches.erase(matches.begin()+25, matches.end());

    Mat imageMatches;
    drawMatches(frame, keypoints1, frameROI, keypoints2, matches, imageMatches, Scalar(255,255,255));

    imshow("Matched", imageMatches);
  }
  gRois = rois;
}

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

  for( size_t i = 0; i < faces.size(); i++ )
  {
    cv::rectangle(frame, cvPoint(faces[i].x, faces[i].y), cvPoint(faces[i].x + faces[i].width, faces[i].y + faces[i].height), CV_RGB(255, 0, 0), 4, 8, 0);
  }
  
  if(faces.size() > 0) 
    trackDetectionOpticalFlow(frame);
  
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
