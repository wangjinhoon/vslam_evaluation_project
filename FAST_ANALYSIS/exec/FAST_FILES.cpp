#include <iostream>
#include "opencv2/opencv.hpp"
#include <vector>
#include <string>

int main(int argv, char** argc){
    cv::Mat img1 = cv::imread("../resources/000000.png", cv::IMREAD_GRAYSCALE);
    cv::Mat img2 = cv::imread("../resources/000006.png", cv::IMREAD_GRAYSCALE);

    // img generation for visualization
    cv::Mat thresh20_1 = img1.clone();
    cv::cvtColor(thresh20_1, thresh20_1, cv::COLOR_GRAY2BGR);
    cv::Mat thresh30_1 = thresh20_1.clone();


    cv::Mat thresh20_2 = img2.clone();
    cv::cvtColor(thresh20_2, thresh20_2, cv::COLOR_GRAY2BGR);
    cv::Mat thresh30_2 = thresh20_2.clone();

    if (img1.empty() || img2.empty()){
        std::cerr << "image load failed" << std::endl;
        return 1;
    }

    // keypoints, descriptor list
    std::vector<cv::KeyPoint> keypoints1, keypoints2, keypoints3, keypoints4;
    cv::Mat desc1, desc2, desc3, desc4;

    // ORB class create
    cv::Ptr<cv::Feature2D> feature_20 = cv::ORB::create(10000, 1.200000048F, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);
    cv::Ptr<cv::Feature2D> feature_30 = cv::ORB::create(10000, 1.200000048F, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 30);

    // get features and descriptors
    feature_20->detectAndCompute(img1, cv::Mat(), keypoints1, desc1);
    feature_30->detectAndCompute(img1, cv::Mat(), keypoints2, desc2);
    feature_20->detectAndCompute(img2, cv::Mat(), keypoints3, desc3);
    feature_30->detectAndCompute(img2, cv::Mat(), keypoints4, desc4);

    // descripotr matching
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::BFMatcher::create(cv::NORM_HAMMING);

    std::vector<cv::DMatch> matches1, matches2;
    matcher->match(desc1, desc3, matches1);
    matcher->match(desc2, desc4, matches2);
    
    // picking good match case, 10%
    float percentage = 0.10f;
    std::sort(matches1.begin(), matches1.end());
    std::sort(matches2.begin(), matches2.end());
    std::vector<cv::DMatch> good_matches1(matches1.begin(), matches1.begin() + (int)(percentage * matches1.size()));
    std::vector<cv::DMatch> good_matches2(matches2.begin(), matches2.begin() + (int)(percentage * matches2.size()));

    // print # of features
    std::cout << "number of features" << std::endl;
    std::cout << "Image 1 feature 20: " << keypoints1.size() << std::endl;
    std::cout << "Image 1 feature 30: " << keypoints2.size() << std::endl;
    std::cout << "Image 2 feature 20: " << keypoints3.size() << std::endl;
    std::cout << "Image 2 feature 30: " << keypoints4.size() << std::endl;

    // average response
    std::vector<double> good_responeses = {0,0,0,0};

    std::vector<cv::Point2f> pt20_1, pt20_2, pt30_1, pt30_2;
    for (size_t i = 0; i < good_matches1.size(); ++i){
        pt20_1.push_back(keypoints1[good_matches1[i].queryIdx].pt);
        pt20_2.push_back(keypoints3[good_matches1[i].queryIdx].pt);
        good_responeses[0] += keypoints1[good_matches1[i].queryIdx].response;
        good_responeses[2] += keypoints3[good_matches1[i].queryIdx].response;
    }
    for (size_t i = 0; i < good_matches2.size(); ++i){
        pt30_1.push_back(keypoints2[good_matches2[i].queryIdx].pt);
        pt30_2.push_back(keypoints4[good_matches2[i].queryIdx].pt);
        good_responeses[1] += keypoints2[good_matches1[i].queryIdx].response;
        good_responeses[3] += keypoints4[good_matches1[i].queryIdx].response;
    }
    good_responeses[0] /= good_matches1.size();
    good_responeses[2] /= good_matches1.size();
    good_responeses[1] /= good_matches2.size();
    good_responeses[3] /= good_matches2.size();

    //
    for (auto response : good_responeses){
        std::cout << response << " ";
    }
    std::cout << std::endl;
    

    cv::Mat H1 = cv::findHomography(pt20_1, pt20_2, cv::RANSAC);
    cv::Mat H2 = cv::findHomography(pt30_1, pt30_2, cv::RANSAC);

    std::vector<cv::Point2f> pt_b_20, pt_b_30, pt_a_20, pt_a_30;
    pt_b_20.reserve(matches1.size());
    pt_b_30.reserve(matches2.size());

    for (size_t i = 0; i < (int)(matches1.size()); ++i){
        pt_b_20.push_back(keypoints1[matches1[i].queryIdx].pt);
    }
    for (size_t i = 0; i < (int)(matches2.size()); ++i){
        pt_b_30.push_back(keypoints2[matches2[i].queryIdx].pt);
    }

    cv::perspectiveTransform(pt_b_20, pt_a_20, H1);
    cv::perspectiveTransform(pt_b_30, pt_a_30, H2);

    for (int i=0; i<10; ++i){
        std::cout << pt_a_20[i] << std::endl;
        std::cout << keypoints3[matches1[i].queryIdx].pt << std::endl;
    }

    // draw matches
    cv::Mat d_matches_20, d_matches_30;
    cv::drawMatches(thresh20_1, keypoints1, thresh20_2, keypoints3, good_matches1, d_matches_20, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    cv::imshow("matches for threshold 20", d_matches_20);
    cv::waitKey();
    // cv::FAST(img1, keypoints1, 20, true);
    // cv::FAST(img1, keypoints2, 30, true);
    // cv::FAST(img2, keypoints3, 20, true);
    // cv::FAST(img2, keypoints4, 30, true);

    // std::cout << "Number of features" << std::endl;
    // std::cout << "threshold 20: " << keypoints1.size() << std::endl;
    // std::cout << "threshold 30: " << keypoints2.size() << std::endl;
    // std::cout << "threshold 20: " << keypoints3.size() << std::endl;
    // std::cout << "threshold 30: " << keypoints4.size() << std::endl;

    // for(cv::KeyPoint kp: keypoints1)
    // {
    //     cv::Point pt(round(kp.pt.x), round(kp.pt.y));
    //     cv::circle(thresh20, pt, 3, cv::Scalar(0,0,255));
    // }
    // for(cv::KeyPoint kp: keypoints2)
    // {
    //     cv::Point pt(round(kp.pt.x), round(kp.pt.y));
    //     cv::circle(thresh30, pt, 3, cv::Scalar(0,0,255));
    // }


    // std::string str1 = "Number of Features ";
    // str1.append(std::to_string(keypoints1.size()));
    // std::string str2 = "Number of Features ";
    // str2.append(std::to_string(keypoints2.size()));
    // cv::putText(thresh20, str1, cv::Point(10,360), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255,0,0), 3);
    // cv::putText(thresh30, str2, cv::Point(10,360), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255,0,0), 3);
    
    // cv::imshow("threshold: 20", thresh20);
    // cv::imshow("threshold: 30", thresh30);

    // cv::waitKey();


    return 0;
}