//
// Created by changh95 on 5/26/22.
//

#ifndef SVSLAM_CLASS_HPP
#define SVSLAM_CLASS_HPP

#define USING_EASY_PROFILER
#include "easy/profiler.h"
#include "spdlog/spdlog.h"
#include "opencv2/opencv.hpp"
#include <vector>

using namespace std;
using namespace cv;

void featureDetection(Mat img, vector<Point2f>& points)	{
  Ptr<Feature2D> feature = ORB::create();

  vector<KeyPoint> keypoints;
  feature->detect(img, keypoints);

  KeyPoint::convert(keypoints, points, vector<int>());
}

void featureTracking(Mat img_1, Mat img_2, vector<Point2f>& points1, vector<Point2f>& points2, vector<uchar>& status)	{

  //this function automatically gets rid of points for which tracking fails

  vector<float> err;
  Size winSize=Size(21,21);
  TermCriteria termcrit=TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01);

  calcOpticalFlowPyrLK(img_1, img_2, points1, points2, status, err, winSize, 3, termcrit, 0, 0.001);

  //getting rid of points for which the KLT tracking failed or those who have gone outside the frame
  int indexCorrection = 0;
  for( int i=0; i<status.size(); i++)
  {  Point2f pt = points2.at(i- indexCorrection);
    if ((status.at(i) == 0)||(pt.x<0)||(pt.y<0)){
      if((pt.x<0)||(pt.y<0))	{
        status.at(i) = 0;
      }
      points1.erase (points1.begin() + (i - indexCorrection));
      points2.erase (points2.begin() + (i - indexCorrection));
      indexCorrection++;
    }

  }

}

#endif  // SVSLAM_CLASS_HPP
