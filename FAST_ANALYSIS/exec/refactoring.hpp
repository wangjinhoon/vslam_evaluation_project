#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>
#include <thread>
#include <cmath>
#include <algorithm>

class FEATURE_compare{
private:
    int NoF1, NoF2;
    double response1, response2;
    double time1, time2;
    cv::Mat src, dst;
    int thresh;
    std::vector<cv::KeyPoint> kp1, kp2;

public:
    FEATURE_compare(cv::Mat src, cv::Mat dst, int thresh);
    void get_feature_info();
    
    int get_NoF1();
    int get_NoF2();
    double get_response1();
    double get_response2();
    double get_time1();
    double get_time2();
    void change_thresh(int thresh_);
    std::vector<cv::KeyPoint> get_kp1();
    std::vector<cv::KeyPoint> get_kp2();
    
};

class FEATURE_match{
private:
    cv::Mat src, dst;
    std::vector<cv::KeyPoint> kp1, kp2;
    cv::Ptr<cv::Feature2D> features;
    cv::Ptr<cv::DescriptorMatcher> matcher;

    cv::Mat desc1, desc2;

    std::vector<cv::DMatch> matches;
    std::vector<cv::DMatch> good_matches;
    cv::Mat H;
    double accuracy;
    int matching_cnt;
    double matching_time;
    double homography_time;
    std::vector<double> desc_times;

    double inlier_thresh;
    double dist_thresh;
    double NofGM_over_NofAM;

public:
    FEATURE_match(cv::Mat src, cv::Mat dst,
    std::vector<cv::KeyPoint> kp1, std::vector<cv::KeyPoint> kp2);

    std::vector<double> time_compute();
    void get_match_info();

    int get_matching_cnt();
    double get_accuracy();
    double get_matching_time();
    double get_homography_time();
    double get_NofGM_over_NofAM();
    std::vector<double> get_desc_time();

};