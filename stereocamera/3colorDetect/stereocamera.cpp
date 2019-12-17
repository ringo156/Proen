#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Labeling.h"
#include "imageProcess.hpp"

#define RIGHT 1
#define LEFT  2
#define DIST_CAMERA 126.0
#define REDUCTION_RATE 0.25

using namespace std;

int main(int argh, char* argv[])
{
    const std::string RIGHT_WIN = "RightCapture";
    const std::string LEFT_WIN  = "LeftCapture";
    const int CAP_WIDTH = 1280;
    const int CAP_HEIGHT = 960;
    const int medianSize = 7;
    double dpi = 3.6 / 1280.0 / REDUCTION_RATE;

    double msec;
    const int samplingNum = 402;
    int count = 0;
    double totaltime = 0;
    long totaltime_ns = 0;
    cv::Mat right_result;
    cv::Mat left_result;
    cv::Point center_r, center_l, point1, point2, d;
    cv::Point blue_center_r, blue_center_l, blue_d;
    cv::Point yellow_center_r, yellow_center_l, yellow_d;
    ImageProcessing impro;
    // ofstream outputfile("test.txt");
    // outputfile << "Z, d, z\n";

    const int Blue_H_max = 125;
    const int Blue_H_min = 86;
    const int Blue_S_max = 255;
    const int Blue_S_min = 122;
    const int Blue_V_max = 255;
    const int Blue_V_min = 56;

    const int Yellow_H_max = 32;
    const int Yellow_H_min = 15;
    const int Yellow_S_max = 255;
    const int Yellow_S_min = 122;
    const int Yellow_V_max = 224;
    const int Yellow_V_min = 56;

    impro.setting(7, 1, cv::MORPH_ERODE, 1, 3);

    //デバイスのオープン
    cv::VideoCapture right(RIGHT);
    cv::VideoCapture left(LEFT);
    // right.open(RIGHT);
    // left.open(LEFT);

    //オープン失敗？
    if(!right.isOpened())
    {
        std::cout << "ERROR: cannot open cam device." << std::endl;
        return -1;
    }
    if(!left.isOpened())
    {
        std::cout << "ERROR: cannot open cam device." << std::endl;
        return -1;
    }

    //キャプチャサイズの設定
    right.set(CV_CAP_PROP_FRAME_WIDTH, CAP_WIDTH);
    right.set(CV_CAP_PROP_FRAME_HEIGHT, CAP_HEIGHT);
    left.set(CV_CAP_PROP_FRAME_HEIGHT, CAP_HEIGHT);
    left.set(CV_CAP_PROP_FRAME_WIDTH, CAP_WIDTH);

    cv::Rect rect(140, 105, 1000, 750); // x, y, width, height
    cv::Mat right_hsv_image;
    cv::Mat left_hsv_image;
    //ループ
    cv::Mat right_frame, right_frame_r;
    cv::Mat left_frame, left_frame_r;
    while(1)
    {
        struct timespec start_val;
	    clock_gettime(CLOCK_MONOTONIC,&start_val);

        cv::Mat right_frame;
        cv::Mat left_frame;
        // 1フレームキャプチャ
        //cap >> frame;
        right >> right_frame;
        left >> left_frame;

        //resize
        // cv::Mat right_frame_r(right_frame, rect);
        cv::resize(right_frame, right_frame_r, cv::Size(), REDUCTION_RATE, REDUCTION_RATE);
        // cv::Mat left_frame_r(left_frame, rect);
        cv::resize(left_frame, left_frame_r, cv::Size(), REDUCTION_RATE, REDUCTION_RATE);

        right_result = impro.extruction(right_frame_r);
        left_result  = impro.extruction(left_frame_r);

        cv::Mat right_red;
        cv::Mat left_red;

        // for red
        cvtColor(right_frame_r, right_hsv_image, CV_BGR2HSV);
        cv::inRange(right_hsv_image,
            cv::Scalar(174, impro.S_min, impro.V_min, 0),
            cv::Scalar(180, impro.S_max, impro.V_max, 0),
            right_red);
        cvtColor(left_frame_r, left_hsv_image, CV_BGR2HSV);
        cv::inRange(left_hsv_image,
            cv::Scalar(174, impro.S_min, impro.V_min, 0),
            cv::Scalar(180, impro.S_max, impro.V_max, 0),
            left_red);

        right_result = right_result + right_red;
        left_result = left_result + left_red;

        // blue
        cv::Mat right_blue;
        cv::Mat left_blue;
        cv::inRange(right_hsv_image,
            cv::Scalar(Blue_H_min, Blue_S_min, Blue_V_min, 0),
            cv::Scalar(Blue_H_max,Blue_S_max,Blue_V_max, 0),
            right_blue);
        cv::inRange(left_hsv_image,
            cv::Scalar(Blue_H_min, Blue_S_min, Blue_V_min, 0),
            cv::Scalar(Blue_H_max,Blue_S_max,Blue_V_max, 0),
            left_blue);

        // yellow
        cv::Mat right_yellow;
        cv::Mat left_yellow;
        cv::inRange(right_hsv_image,
            cv::Scalar(Yellow_H_min, Yellow_S_min, Yellow_V_min, 0),
            cv::Scalar(Yellow_H_max,Yellow_S_max,Yellow_V_max, 0),
            right_yellow);
        cv::inRange(left_hsv_image,
            cv::Scalar(Yellow_H_min, Yellow_S_min, Yellow_V_min, 0),
            cv::Scalar(Yellow_H_max,Yellow_S_max,Yellow_V_max, 0),
            left_yellow);

        // cv::imshow("right_binary", right_result);
        // cv::imshow("blue_binary", right_blue);
        // cv::imshow("yellow_binary", right_yellow);


        // filter prosecc
        // right_result = impro.filter(right_result);
        // left_result = impro.filter(left_result);

        // impro.median_one_ptr(right_result, right_result);
        // impro.median_one_ptr(left_result, left_result);
        // impro.median_one_ptr(right_blue, right_blue);
        // impro.median_one_ptr(left_blue, left_blue);
        //
        // impro.median_one_ptr(right_yellow, right_yellow);
        // impro.median_one_ptr(left_yellow, left_yellow);

        // cv::medianBlur(right_result, right_result, medianSize);
        // cv::medianBlur(left_result, left_result, medianSize);
        // cv::medianBlur(right_blue, right_blue, medianSize);
        // cv::medianBlur(left_blue, left_blue, medianSize);
        // cv::medianBlur(right_yellow, right_yellow, medianSize);
        // cv::medianBlur(left_yellow, left_yellow, medianSize);

        impro.labeling(right_result,1000,20,center_r,point1,point2);
        cv::circle(right_frame_r, center_r, 10, cv::Scalar(0, 0, 255), -1, 1, 0);
        impro.labeling(left_result,1000,20,center_l,point1,point2);
        cv::circle(left_frame_r, center_l, 10, cv::Scalar(0, 0, 255), -1, 1, 0);
        d = center_l - center_r; //視差の計算

        impro.labeling(right_yellow, 500, 10, yellow_center_r, point1, point2);
        cv::circle(right_frame_r, yellow_center_r, 10, cv::Scalar(0, 255, 255), -1, 1, 0);
        impro.labeling(left_yellow, 500, 10, yellow_center_l, point1, point2);
        cv::circle(left_frame_r, yellow_center_l, 10, cv::Scalar(0, 255, 255), -1, 1, 0);
        yellow_d = yellow_center_l - yellow_center_r;

        impro.labeling(right_blue, 1000, 20, blue_center_r, point1, point2);
        cv::circle(right_frame_r, blue_center_r, 10, cv::Scalar(255, 0, 0), -1, 1, 0);
        impro.labeling(left_blue, 1000, 20, blue_center_l, point1, point2);
        cv::circle(left_frame_r, blue_center_l, 10, cv::Scalar(255, 0, 0), -1, 1, 0);
        blue_d = blue_center_l - blue_center_r;

        // cv::imshow("yellow", right_yellow);
        // cv::imshow("blue", right_blue);

        //キー待ち
        int inp_key = cv::waitKey(1);

        double num, dis, blue_dis, yellow_dis;
        dis = DIST_CAMERA * 4.0 / (dpi * d.x);
        blue_dis = DIST_CAMERA * 4.0 / (dpi * blue_d.x);
        yellow_dis = DIST_CAMERA * 4.0 / (dpi * yellow_d.x);
        //dis = dist * 4.0 / (dpi * d.x);
        //入力あり？
        if (inp_key >= 0) {

            // if(inp_key == 'm'){
            //     // Z, d, z
            //     cin >> num;
            //     num = num - 80.0;
            //     outputfile << num << ", " << d.x << ", " << dis <<endl;
            // }
            if (inp_key == 27 || inp_key == 81 || inp_key == 113) break;
        }

        //表示
        cv::imshow(RIGHT_WIN, right_frame_r);
        // cv::imshow("right result", right_result);
        cv::imshow(LEFT_WIN, left_frame_r);
        // cv::imshow("left result", left_result);

        // struct timespec end;
        // clock_gettime(CLOCK_REALTIME,&end);

        // printf("elapsed time = ");
        // if (end.tv_nsec < start_val.tv_nsec) {
        //     long int temp = end.tv_nsec + (long int)1.0e+9 - start_val.tv_nsec;
        //     printf("%5ld.%09ld", end.tv_sec - start_val.tv_sec - 1,
        //          temp);
        //     totaltime += temp;
        // } else {
        //     long int temp = end.tv_nsec - start_val.tv_nsec;
        //     printf("%5ld.%09ld", end.tv_sec - start_val.tv_sec,
        //          temp);
        //          totaltime += temp;
        // }
        // printf(" sec \n");

        struct timespec end;
		clock_gettime(CLOCK_MONOTONIC,&end);
		long sec = end.tv_sec - start_val.tv_sec;
		long nsec = end.tv_nsec-start_val.tv_nsec;
		if(nsec < 0){
			sec--;
			nsec += 1000000000L;
		}
		msec = sec*1000+nsec/1000000;
        // printf("time = %f, nsec = %ld\n", msec, nsec);
        // cout << "time = " << msec << "ms, d.x = " << d.x << "pixel, " << "z = " << dis << "mm" << endl;
        if(count % 10 == 0){
            cout << "time = " << msec << "ms, red = " << dis << "mm, blue = " << blue_dis << "mm, yellow = "<< yellow_dis << "mm" << endl;

        }
        if(count > 4){
            totaltime_ns += nsec;
        }
        if(count > samplingNum){
            printf("time=%ld\n", totaltime_ns);
            printf("time=%f\n", double(totaltime_ns) / (double)samplingNum - 4);
            break;
        }
        count++;
    }
    //ウィンドウ破棄
    cv::destroyAllWindows();
    // outputfile.close();
    return 0;
}
