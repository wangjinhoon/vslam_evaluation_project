#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>
#include <thread>
#include <cmath>
#include <algorithm>

class FEATURE_compare{
private:
    int NoF1, NoF2, NoF3;
    double response1, response2, response3;
    cv::Mat src;
    int thresh1, thresh2, thresh3;
    std::vector<cv::KeyPoint> kp1, kp2, kp3;

public:
    FEATURE_compare(cv::Mat src, int thresh1, int thresh2, int thresh3);
    void get_feature1_info();
    void get_feature2_info();
    void get_feature3_info();
    
    std::vector<cv::KeyPoint> get_kp1();
    std::vector<cv::KeyPoint> get_kp2();
    std::vector<cv::KeyPoint> get_kp3();
};

class FEATURE_match{
private:
    cv::Mat src, dst;
    std::vector<cv::KeyPoint> kp1_1, kp1_2, kp1_3;
    std::vector<cv::KeyPoint> kp2_1, kp2_2, kp2_3;
    cv::Ptr<cv::Feature2D> features;
    cv::Ptr<cv::DescriptorMatcher> matcher;
    cv::Mat desc1_1, desc1_2, desc1_3;
    cv::Mat desc2_1, desc2_2, desc2_3;
    std::vector<cv::DMatch> matches1, matches2, matches3;
    std::vector<cv::DMatch> good_matches1, good_matches2, good_matches3;
    cv::Mat H1, H2, H3;
    std::vector<double> accuracy;
    std::vector<int> matching_cnt;
    std::vector<double> matching_time;
    std::vector<double> homography_time;
    std::vector<double> desc_times;

    double inlier_thresh;
    double dist_thresh;

public:
    FEATURE_match(cv::Mat src, cv::Mat dst,
    std::vector<cv::KeyPoint> kp1_1, std::vector<cv::KeyPoint> kp1_2, std::vector<cv::KeyPoint> kp1_3,
    std::vector<cv::KeyPoint> kp2_1, std::vector<cv::KeyPoint> kp2_2, std::vector<cv::KeyPoint> kp2_3);

    std::vector<double> time_compute();
    void get_match_info();

    std::vector<int> get_matching_cnt();
    std::vector<double> get_accuracy();
    std::vector<double> get_matching_time();
    std::vector<double> get_homography_time();
    std::vector<double> get_desc_time();

};

int main(int argv, char** argc){
    std::string str1, str2;
    if (argv == 2)
    {
        switch(atoi(argc[1])){
            case 1:
                str1 = "../resources/slambook1.jpg";
                str2 = "../resources/slambook2.jpg";
                break;
            case 2:
                str1 = "../resources/kitti_far_01.png";
                str2 = "../resources/kitti_far_02.png";
                break;
            case 3:
                str1 = "../resources/kitti_close_01.png";
                str2 = "../resources/kitti_close_02.png";
                break;
            default:
                str1 = "../resources/slambook1.jpg";
                str2 = "../resources/slambook2.jpg";
                break;
        }  
    }

    cv::Mat img1 = cv::imread(str1, cv::IMREAD_COLOR);
    cv::Mat img2 = cv::imread(str2, cv::IMREAD_COLOR);
    cv::resize(img1, img1, cv::Size(), 0.2, 0.2, cv::INTER_AREA);
    cv::resize(img2, img2, cv::Size(), 0.2, 0.2, cv::INTER_AREA);
    std::cout << "Image cols and rows" << std::endl << img1.cols << ", " << img1.rows << std::endl;
    std::cout << std::endl;
    cv::Mat img1_gray, img2_gray;

    cv::cvtColor(img1, img1_gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(img2, img2_gray, cv::COLOR_BGR2GRAY);


    int thresh1, thresh2, thresh3;
    thresh1 = 10;
    thresh2 = 15;
    thresh3 = 20;
    
    for (int i=0; i<3; ++i){
        FEATURE_compare img1_fast(img1_gray, thresh1, thresh2, thresh3);
        FEATURE_compare img2_fast(img2_gray, thresh1, thresh2, thresh3);

        img1_fast.get_feature1_info();
        img1_fast.get_feature2_info();
        img1_fast.get_feature3_info();
        img2_fast.get_feature1_info();
        img2_fast.get_feature2_info();
        img2_fast.get_feature3_info();

        std::vector<cv::KeyPoint> kp1_1, kp1_2, kp1_3;
        std::vector<cv::KeyPoint> kp2_1, kp2_2, kp2_3;

        kp1_1 = img1_fast.get_kp1();
        kp1_2 = img1_fast.get_kp2();
        kp1_3 = img1_fast.get_kp3();
        kp2_1 = img2_fast.get_kp1();
        kp2_2 = img2_fast.get_kp2();
        kp2_3 = img2_fast.get_kp3();

        FEATURE_match matcher(img1_gray, img2_gray, kp1_1, kp1_2, kp1_3, kp2_1, kp2_2, kp2_3);
        std::vector<double> accuracy = matcher.get_accuracy();
        std::vector<int> matching_cnt = matcher.get_matching_cnt();
        std::vector<double> homography_time = matcher.get_homography_time();
        std::vector<double> matching_time = matcher.get_matching_time();
        std::vector<double> desc_time = matcher.get_desc_time();

        for (int i=0; i<3; ++i){
            std::cout << "====feature " << i+1 << " matching====" << std::endl;
            std::cout << "accuracy: " << accuracy[i] << std::endl;
            std::cout << "matching cnt: " << matching_cnt[i] << std::endl;
            std::cout << "homography_time: " << homography_time[i] << std::endl;
            std::cout << "matching time: " << matching_time[i] << std::endl;
            std::cout << "desc time: " << desc_time[0+i] + desc_time[3+i] << std::endl;
            std::cout << std::endl;
        }
        thresh1 += 15;
        thresh2 += 15;
        thresh3 += 15;
    }

    return 0;
}

FEATURE_compare::FEATURE_compare(cv::Mat src, int thresh1, int thresh2, int thresh3):
src(src), thresh1(thresh1), thresh2(thresh2), thresh3(thresh3)
{
}

void FEATURE_compare::get_feature1_info(){
    std::vector<cv::KeyPoint> temp_keypoint;
    // cv::Mat temp_desc;

    std::chrono::steady_clock::time_point start_t = std::chrono::steady_clock::now();
    for (int i = 0; i < 10; ++i){
        cv::FAST(src, temp_keypoint, thresh1);
    }
    std::chrono::steady_clock::time_point end_t = std::chrono::steady_clock::now();

    kp1 = temp_keypoint;
    // desc1 = temp_desc;
    NoF1 = kp1.size();

    double min= 10000, max = -10000;

    for (auto kp : kp1){
        if (kp.response < min) min = kp.response;
        if (kp.response > max) max = kp.response;
        response1 += kp.response;
    }
    response1/=NoF1;

    std::cout << "=====feature1=====" << std::endl;
    std::cout << "Fast threshold: " << thresh1 << std::endl;
    std::cout << "NoF: " << NoF1 << std::endl;;
    std::cout << "min response: " << min << std::endl;
    std::cout << "max response: " << max << std::endl;
    std::cout << "Average responses: " << response1 << std::endl;
    std::cout << "Average time[us]: " << std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0 << std::endl;
    std::cout << std::endl;
}
void FEATURE_compare::get_feature2_info(){
    std::vector<cv::KeyPoint> temp_keypoint;
    // cv::Mat temp_desc;

    std::chrono::steady_clock::time_point start_t = std::chrono::steady_clock::now();
    for (int i = 0; i < 10; ++i){
        cv::FAST(src, temp_keypoint, thresh2);
    }
    std::chrono::steady_clock::time_point end_t = std::chrono::steady_clock::now();
    kp2 = temp_keypoint;
    // desc1 = temp_desc;
    NoF2 = kp2.size();

    double min= 10000, max = -10000;
    for (auto kp : kp2){
        if (kp.response < min) min = kp.response;
        if (kp.response > max) max = kp.response;
        response2 += kp.response;
    }
    response2/=NoF2;

    std::cout << "=====feature2=====" << std::endl;
    std::cout << "Fast threshold: " << thresh2 << std::endl;
    std::cout << "NoF: " << NoF2 << std::endl;;
    std::cout << "min response: " << min << std::endl;
    std::cout << "max response: " << max << std::endl;
    std::cout << "Average responses: " << response2 << std::endl;
    std::cout << "Average time[us]: " << std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0 << std::endl;
    std::cout << std::endl;
}
void FEATURE_compare::get_feature3_info(){
    std::vector<cv::KeyPoint> temp_keypoint;
    // cv::Mat temp_desc;

    std::chrono::steady_clock::time_point start_t = std::chrono::steady_clock::now();
    for (int i = 0; i < 10; ++i){
        cv::FAST(src, temp_keypoint, thresh3);
    }
    std::chrono::steady_clock::time_point end_t = std::chrono::steady_clock::now();
    kp3 = temp_keypoint;
    // desc1 = temp_desc;
    NoF3 = kp3.size();

    double min= 10000, max = -10000;
    for (auto kp : kp3){
        if (kp.response < min) min = kp.response;
        if (kp.response > max) max = kp.response;
        response3 += kp.response;
    }
    response3/=NoF3;

    std::cout << "=====feature3=====" << std::endl;
    std::cout << "Fast threshold: " << thresh3 << std::endl;
    std::cout << "NoF: " << NoF3 << std::endl;;
    std::cout << "min response: " << min << std::endl;
    std::cout << "max response: " << max << std::endl;
    std::cout << "Average responses: " << response3 << std::endl;
    std::cout << "Average time[us]: " << std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0 << std::endl;
    std::cout << std::endl;
}


std::vector<cv::KeyPoint> FEATURE_compare::get_kp1(){
    return kp1;
}
std::vector<cv::KeyPoint> FEATURE_compare::get_kp2(){
    return kp2;
}
std::vector<cv::KeyPoint> FEATURE_compare::get_kp3(){
    return kp3;
}

FEATURE_match::FEATURE_match(cv::Mat src, cv::Mat dst,
    std::vector<cv::KeyPoint> kp1_1, std::vector<cv::KeyPoint> kp1_2, std::vector<cv::KeyPoint> kp1_3,
    std::vector<cv::KeyPoint> kp2_1, std::vector<cv::KeyPoint> kp2_2, std::vector<cv::KeyPoint> kp2_3):
    src(src), dst(dst), kp1_1(kp1_1), kp1_2(kp1_2), kp1_3(kp1_3), kp2_1(kp2_1), kp2_2(kp2_2), kp2_3(kp2_3)
{
    features = cv::ORB::create();
    matcher = cv::BFMatcher::create(cv::NORM_HAMMING);
    inlier_thresh = 0.02;
    dist_thresh = std::min(src.cols, src.rows) * inlier_thresh;
    get_match_info();
}

void FEATURE_match::get_match_info(){
    desc_times = time_compute();
    matching_time.clear();

    std::chrono::steady_clock::time_point start_t, end_t;
    start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        matcher->match(desc1_1, desc2_1, matches1);
    }
    end_t = std::chrono::steady_clock::now();
    matching_time.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        matcher->match(desc1_2, desc2_2, matches2);
    }
    end_t = std::chrono::steady_clock::now();
    matching_time.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        matcher->match(desc1_3, desc2_3, matches3);
    }
    end_t = std::chrono::steady_clock::now();
    matching_time.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    // picking good match case, 10%
    float percentage = 0.10f;
    std::sort(matches1.begin(), matches1.end());
    std::sort(matches2.begin(), matches2.end());
    std::sort(matches3.begin(), matches3.end());

    std::vector<cv::DMatch> good_matches1(matches1.begin(), matches1.begin() + (int)(percentage * matches1.size()));
    std::vector<cv::DMatch> good_matches2(matches2.begin(), matches2.begin() + (int)(percentage * matches2.size()));
    std::vector<cv::DMatch> good_matches3(matches3.begin(), matches3.begin() + (int)(percentage * matches3.size()));

    std::vector<cv::Point2f> pt1_1, pt1_2, pt1_3, pt2_1, pt2_2, pt2_3;
    for (size_t i = 0; i < good_matches1.size(); ++i){
        pt1_1.push_back(kp1_1[good_matches1[i].queryIdx].pt);
        pt2_1.push_back(kp2_1[good_matches1[i].queryIdx].pt);
    }
    for (size_t i = 0; i < good_matches2.size(); ++i){
        pt1_2.push_back(kp1_2[good_matches2[i].queryIdx].pt);
        pt2_2.push_back(kp2_2[good_matches2[i].queryIdx].pt);
    }
    for (size_t i = 0; i < good_matches3.size(); ++i){
        pt1_3.push_back(kp1_3[good_matches3[i].queryIdx].pt);
        pt2_3.push_back(kp2_3[good_matches3[i].queryIdx].pt);
    }

    start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        H1 = cv::findHomography(pt1_1, pt2_1, cv::RANSAC);
    }
    end_t = std::chrono::steady_clock::now();
    homography_time.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        H2 = cv::findHomography(pt1_2, pt2_2, cv::RANSAC);
    }
    end_t = std::chrono::steady_clock::now();
    homography_time.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        H3 = cv::findHomography(pt1_3, pt2_3, cv::RANSAC);
    }
    end_t = std::chrono::steady_clock::now();
    homography_time.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    std::vector<cv::Point2f> pt2_1_est, pt2_2_est, pt2_3_est;
    std::vector<cv::Point2f> pt1_1_b, pt1_2_b, pt1_3_b;
    pt1_1_b.reserve(matches1.size());
    pt1_2_b.reserve(matches2.size());
    pt1_3_b.reserve(matches3.size());

    for (size_t i = 0; i < (int)(matches1.size()); ++i){
        pt1_1_b.push_back(kp1_1[matches1[i].queryIdx].pt);
    }
    for (size_t i = 0; i < (int)(matches2.size()); ++i){
        pt1_2_b.push_back(kp1_2[matches2[i].queryIdx].pt);
    }
    for (size_t i = 0; i < (int)(matches3.size()); ++i){
        pt1_3_b.push_back(kp1_3[matches3[i].queryIdx].pt);
    }

    cv::perspectiveTransform(pt1_1_b, pt2_1_est, H1);
    cv::perspectiveTransform(pt1_2_b, pt2_2_est, H2);
    cv::perspectiveTransform(pt1_3_b, pt2_3_est, H3);

    double dist;
    int inlier_match1 = 0, inlier_match2 = 0, inlier_match3 = 0;
    for (size_t i = 0; i<(int)(matches1.size()); ++i){
        dist = pow(pt2_1[matches1[i].queryIdx].x - pt2_1_est[i].x, 2)
                + pow(pt2_1[matches1[i].queryIdx].y - pt2_1_est[i].y, 2);
        dist = pow(dist, 0.5);
        if (dist > dist_thresh) inlier_match1++;
    }

    for (size_t i = 0; i<(int)(matches2.size()); ++i){
        dist = pow(pt2_2[matches2[i].queryIdx].x - pt2_2_est[i].x, 2)
                + pow(pt2_2[matches2[i].queryIdx].y - pt2_2_est[i].y, 2);
        dist = pow(dist, 0.5);
        if (dist > dist_thresh) inlier_match2++;
    }

    for (size_t i = 0; i<(int)(matches3.size()); ++i){
        dist = pow(pt2_3[matches3[i].queryIdx].x - pt2_3_est[i].x, 2)
                + pow(pt2_3[matches3[i].queryIdx].y - pt2_3_est[i].y, 2);
        dist = pow(dist, 0.5);
        if (dist > dist_thresh) inlier_match3++;
    }

    accuracy.push_back((double)inlier_match1/matches1.size());
    accuracy.push_back((double)inlier_match2/matches2.size());
    accuracy.push_back((double)inlier_match3/matches3.size());
    matching_cnt.push_back(matches1.size());
    matching_cnt.push_back(matches2.size());
    matching_cnt.push_back(matches3.size());
}

std::vector<double> FEATURE_match::time_compute(){
    std::vector<double> times;

    std::chrono::steady_clock::time_point start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        features->compute(src, kp1_1, desc1_1);
    }
    std::chrono::steady_clock::time_point end_t = std::chrono::steady_clock::now();
    times.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        features->compute(src, kp1_2, desc1_2);
    }
    end_t = std::chrono::steady_clock::now();
    times.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        features->compute(src, kp1_3, desc1_3);
    }
    end_t = std::chrono::steady_clock::now();
    times.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        features->compute(dst, kp2_1, desc2_1);
    }
    end_t = std::chrono::steady_clock::now();
    times.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        features->compute(dst, kp2_2, desc2_2);
    }
    end_t = std::chrono::steady_clock::now();
    times.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        features->compute(dst, kp2_3, desc2_3);
    }
    end_t = std::chrono::steady_clock::now();
    times.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    return times;
}

std::vector<int> FEATURE_match::get_matching_cnt(){
    return matching_cnt;
}

std::vector<double> FEATURE_match::get_accuracy(){
    return accuracy;
}
std::vector<double> FEATURE_match::get_matching_time(){
    return matching_time;
}
std::vector<double> FEATURE_match::get_homography_time(){
    return homography_time;
}
std::vector<double> FEATURE_match::get_desc_time(){
    return desc_times;
}