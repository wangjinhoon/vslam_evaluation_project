#include "refactoring.hpp"

int main(int argv, char** argc){
    std::string str1, str2;
    double resize_factor = 1;
    if (argv == 2)
    {
        switch(atoi(argc[1])){
            case 0:
                str1 = "../resources/slambook1.jpg";
                str2 = "../resources/slambook2.jpg";
                resize_factor = 0.5;
                break;
            case 1:
                str1 = "../resources/slambook1.jpg";
                str2 = "../resources/slambook2.jpg";
                resize_factor = 0.25;
                break;
            case 2:
                str1 = "../resources/kitti_far_01.png";
                str2 = "../resources/kitti_far_02.png";
                resize_factor = 0.8;
                break;
            case 3:
                str1 = "../resources/kitti_far_01.png";
                str2 = "../resources/kitti_far_02.png";
                resize_factor = 0.6;
                break;
            case 4:
                str1 = "../resources/kitti_far_01.png";
                str2 = "../resources/kitti_far_02.png";
                resize_factor = 0.4;
                break;
            case 5:
                str1 = "../resources/kitti_far_01.png";
                str2 = "../resources/kitti_far_02.png";
                resize_factor = 0.2;
                break;
            // case 4:
            //     str1 = "../resources/kitti_close_01.png";
            //     str2 = "../resources/kitti_close_02.png";
            //     resize_factor = 1;
            //     break;
            // case 5:
            //     str1 = "../resources/kitti_close_01.png";
            //     str2 = "../resources/kitti_close_02.png";
            //     resize_factor = 0.8;
            //     break;
            default:
                str1 = "../resources/slambook1.jpg";
                str2 = "../resources/slambook2.jpg";
                resize_factor = 0.5;
                break;
        }  
    }
    else{
            str1 = "../resources/slambook1.jpg";
            str2 = "../resources/slambook2.jpg";
            resize_factor = 0.5;
    }

    std::cout << "image loading: " << std::endl << str1 << std::endl << str2 << std::endl;

    cv::Mat img1 = cv::imread(str1, cv::IMREAD_COLOR);
    cv::Mat img2 = cv::imread(str2, cv::IMREAD_COLOR);
    cv::resize(img1, img1, cv::Size(), resize_factor, resize_factor, cv::INTER_AREA);
    cv::resize(img2, img2, cv::Size(), resize_factor, resize_factor, cv::INTER_AREA);
    std::cout << "Image cols and rows" << std::endl << img1.cols << ", " << img1.rows << std::endl;
    std::cout << std::endl;
    cv::Mat img1_gray, img2_gray;

    cv::cvtColor(img1, img1_gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(img2, img2_gray, cv::COLOR_BGR2GRAY);

    for (int i=0; i<8; ++i){
        int thresh = 10 + 5 * i;
        FEATURE_compare img_fast(img1_gray, img2_gray, thresh);

        std::vector<cv::KeyPoint> kp1, kp2;
        kp1 = img_fast.get_kp1();
        kp2 = img_fast.get_kp2();
        int NoF1 = img_fast.get_NoF1();
        int NoF2 = img_fast.get_NoF2();
        double response1 = img_fast.get_response1();
        double response2 = img_fast.get_response2();
        double time1 = img_fast.get_time1();
        double time2 = img_fast.get_time2();

        FEATURE_match matcher(img1_gray, img2_gray, kp1, kp2);
        double accuracy = matcher.get_accuracy();
        int matching_cnt = matcher.get_matching_cnt();
        double homography_time = matcher.get_homography_time();
        double matching_time = matcher.get_matching_time();
        double NofGM_over_NofAM = matcher.get_NofGM_over_NofAM();
        std::vector<double> desc_time = matcher.get_desc_time();

        std::cout << "====thresh " << thresh << " matching====" << std::endl;
        std::cout << "1st NoF:           " << NoF1 << std::endl;
        std::cout << "1st response:      " << response1 << std::endl;
        std::cout << "1st time:          " << time1 << std::endl;
        std::cout << "2nd NoF:           " << NoF2 << std::endl;
        std::cout << "2nd response:      " << response2 << std::endl;
        std::cout << "2nd time:          " << time2 << std::endl;

        std::cout << "Accuracy:          " << accuracy << std::endl;
        std::cout << "matching accuracy: " << NofGM_over_NofAM  << " %" << std::endl;
        std::cout << "Matching cnt:      " << matching_cnt << std::endl;
        std::cout << "Desc time:         " << desc_time[0] + desc_time[1] << std::endl;
        std::cout << "Matching time:     " << matching_time << std::endl;
        std::cout << "Homography time:   " << homography_time << std::endl;
        std::cout << std::endl;
    }

    return 0;
}