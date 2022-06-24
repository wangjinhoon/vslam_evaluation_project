#include "refactoring.hpp"

FEATURE_compare::FEATURE_compare(cv::Mat src, cv::Mat dst, int thresh):
src(src), dst(dst), thresh(thresh)
{
    get_feature_info();
}

void FEATURE_compare::get_feature_info(){
    std::vector<cv::KeyPoint> temp_keypoint;
    // cv::Mat temp_desc;

    std::chrono::steady_clock::time_point start_t = std::chrono::steady_clock::now();
    for (int i = 0; i < 10; ++i){
        cv::FAST(src, temp_keypoint, thresh);
    }
    std::chrono::steady_clock::time_point end_t = std::chrono::steady_clock::now();
    time1 = std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0;
    kp1 = temp_keypoint;

    start_t = std::chrono::steady_clock::now();
    for (int i = 0; i < 10; ++i){
        cv::FAST(dst, temp_keypoint, thresh);
    }
    end_t = std::chrono::steady_clock::now();
    time2 = std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0;
    kp2 = temp_keypoint;

    // desc1 = temp_desc;
    NoF1 = kp1.size();
    NoF2 = kp2.size();

    response1 = 0;
    response2 = 0;
    for (auto kp : kp1){
        response1 += kp.response;
    }
    response1/=NoF1;

    for (auto kp : kp2){
        response2 += kp.response;
    }
    response2/=NoF2;
}

std::vector<cv::KeyPoint> FEATURE_compare::get_kp1(){
    return kp1;
}
std::vector<cv::KeyPoint> FEATURE_compare::get_kp2(){
    return kp2;
}
int FEATURE_compare::get_NoF1(){
    return NoF1;
}
int FEATURE_compare::get_NoF2(){
    return NoF2;
}
double FEATURE_compare::get_response1(){
    return response1;
}
double FEATURE_compare::get_response2(){
    return response2;
}

double FEATURE_compare::get_time1(){
    return time1;
}
double FEATURE_compare::get_time2(){
    return time2;
}
void FEATURE_compare::change_thresh(int thresh_)
{
    thresh = thresh_;
}


FEATURE_match::FEATURE_match(cv::Mat src, cv::Mat dst,
    std::vector<cv::KeyPoint> kp1, std::vector<cv::KeyPoint> kp2):
    src(src), dst(dst), kp1(kp1), kp2(kp2)
{
    features = cv::ORB::create();
    matcher = cv::BFMatcher::create(cv::NORM_HAMMING);
    inlier_thresh = 0.05;
    dist_thresh = std::min(src.cols, src.rows) * inlier_thresh;
    get_match_info();
}

void FEATURE_match::get_match_info(){
    desc_times = time_compute();
    matching_time = 0;

    std::chrono::steady_clock::time_point start_t, end_t;
    start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        matcher->match(desc1, desc2, matches);
    }
    end_t = std::chrono::steady_clock::now();
    matching_time = (std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    // picking good match case, 10%
    float percentage = 0.10f;
    std::sort(matches.begin(), matches.end());

    int temp_good_match = 0;
    for (size_t i = 0; i < matches.size(); ++i){
        // std::cout << matches[i].distance << " ";
        if (matches[i].distance < 64) temp_good_match++;
    }
    NofGM_over_NofAM = std::round((double)temp_good_match/matches.size() * 100);

    std::vector<cv::DMatch> good_matches(matches.begin(), matches.begin() + (int)(percentage * matches.size()));

    std::vector<cv::Point2f> pt1, pt2;
    for (size_t i = 0; i < good_matches.size(); ++i){
        pt1.push_back(kp1[good_matches[i].queryIdx].pt);
        pt2.push_back(kp2[good_matches[i].queryIdx].pt);
    }

    homography_time = 0;
    accuracy = 0;
    matching_cnt = matches.size();
    std::vector<cv::Point2f> pt2_est;
    std::vector<cv::Point2f> pt1_all;
    pt1_all.reserve(matches.size());

    for (size_t i = 0; i < (int)(matches.size()); ++i){
        pt1_all.push_back(kp1[matches[i].queryIdx].pt);
    }

    for(int i=0; i<10; ++i){
        pt2_est.clear();
        start_t = std::chrono::steady_clock::now();
        H = cv::findHomography(pt1, pt2, cv::RANSAC);
        end_t = std::chrono::steady_clock::now();
        homography_time += (std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

        cv::perspectiveTransform(pt1_all, pt2_est, H);
        double dist = 0;
        int inlier_match = 0;

        for (size_t i = 0; i<(int)(matches.size()); ++i){
            // dist = pow(pt2[matches[i].queryIdx].x - pt2_est[i].x, 2)
            //         + pow(pt2[matches[i].queryIdx].y - pt2_est[i].y, 2);
            // dist = pow(dist, 0.5);
            dist = abs(pt2[matches[i].queryIdx].y - pt2_est[i].y);
            if (dist < dist_thresh) inlier_match++;
        }
        
        accuracy += (double)inlier_match/matches.size();
    }

    homography_time /= 10;
    accuracy /= 10;
}

std::vector<double> FEATURE_match::time_compute(){
    std::vector<double> times;

    std::chrono::steady_clock::time_point start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        features->compute(src, kp1, desc1);
    }
    std::chrono::steady_clock::time_point end_t = std::chrono::steady_clock::now();
    times.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    start_t = std::chrono::steady_clock::now();
    for(int i=0; i<10; ++i){
        features->compute(src, kp2, desc2);
    }
    end_t = std::chrono::steady_clock::now();
    times.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end_t-start_t).count()/10.0);

    return times;
}

int FEATURE_match::get_matching_cnt(){
    return matching_cnt;
}

double FEATURE_match::get_accuracy(){
    return accuracy;
}

double FEATURE_match::get_matching_time(){
    return matching_time;
}

double FEATURE_match::get_homography_time(){
    return homography_time;
}

double FEATURE_match::get_NofGM_over_NofAM(){
    return NofGM_over_NofAM;
}

std::vector<double> FEATURE_match::get_desc_time(){
    return desc_times;
}