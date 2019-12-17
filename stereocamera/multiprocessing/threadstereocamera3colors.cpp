#include <iostream>
#include <thread>
#include <time.h>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "stereocamera.hpp"

#define RIGHT 1
#define LEFT  2

#define CAP_WIDTH  1280
#define CAP_HEIGHT  960

#define MEDIAN_SIZE 7

using namespace std;

class RedProcess : public ImageProcessing{
private:
    cv::Mat red_mask;

public:
    RedProcess(const int _ksize, const bool _trackBar, const HSV_threshold _thr)
    : ImageProcessing(_ksize ,_trackBar, _thr)
    {
    }
    void colorExtruction(cv::Mat& src, cv::Mat& dist){
        cv::cvtColor(src, red_mask, CV_BGR2HSV);

        cv::inRange(red_mask,
            cv::Scalar(H_min, S_min, V_min, 0),
            cv::Scalar(H_max, S_max, V_max, 0),
            dist);

        // for red
        cv::inRange(red_mask,
            cv::Scalar(174, S_min, V_min, 0),
            cv::Scalar(180, S_max, V_max, 0),
            red_mask);

        dist = dist + red_mask;

    }
};


cv::VideoCapture cap_r(RIGHT);
cv::VideoCapture cap_l(LEFT);

cv::Mat frame_r, frame_l, right_result, left_result;
cv::Point red_center_r, red_center_l, blue_center_r, blue_center_l;
cv::Point yellow_center_r, yellow_center_l;

uint32_t g_rightEndFlg = 0;
uint32_t g_leftEndFlg = 0;
uint32_t g_msg = 0;

void GetFrameRightCamera() {

    HSV_threshold blue_hsv_thr, red_hsv_thr, yellow_hsv_thr;

    blue_hsv_thr.H_max = 143;
    blue_hsv_thr.H_min = 105;
    blue_hsv_thr.S_max = 255;
    blue_hsv_thr.S_min = 86;
    blue_hsv_thr.V_max = 220;
    blue_hsv_thr.V_min = 61;

    red_hsv_thr.H_max = 15;
    red_hsv_thr.H_min = 0;
    red_hsv_thr.S_max = 255;
    red_hsv_thr.S_min = 86;
    red_hsv_thr.V_max = 255;
    red_hsv_thr.V_min = 61;

    yellow_hsv_thr.H_max = 34;
    yellow_hsv_thr.H_min = 17;
    yellow_hsv_thr.S_max = 255;
    yellow_hsv_thr.S_min = 98;
    yellow_hsv_thr.V_max = 255;
    yellow_hsv_thr.V_min = 107;

    ImageProcessing yellow_right(MEDIAN_SIZE, false, yellow_hsv_thr);
    ImageProcessing blue_right(MEDIAN_SIZE, false, blue_hsv_thr);
    RedProcess red_right(MEDIAN_SIZE, false, red_hsv_thr);
    cv::Mat source;

    while (true) {
        // std::lock_guard<std::mutex> lock(mtx);
        // printf("\nRead Thread\n");
        cap_r >> source;

        cv::resize(source, frame_r, cv::Size(), 0.5, 0.5);
        yellow_right.mainProcess(frame_r);
        blue_right.mainProcess(frame_r);
        red_right.mainProcess(frame_r);
        // right_result = yellow_right.getImg(RESULT_IMAGE);
        yellow_center_r = yellow_right.getCenter();
        blue_center_r = blue_right.getCenter();
        red_center_r = red_right.getCenter();

        cv::circle(frame_r, yellow_center_r, 10, cv::Scalar(0, 255, 255), -1, 1, 0);

        g_rightEndFlg = 1;

        if(g_msg == 1){
            break;
        }
    }
}

void GetFrameLeftCamera() {
    HSV_threshold blue_hsv_thr, red_hsv_thr, yellow_hsv_thr;

    blue_hsv_thr.H_max = 143;
    blue_hsv_thr.H_min = 105;
    blue_hsv_thr.S_max = 255;
    blue_hsv_thr.S_min = 86;
    blue_hsv_thr.V_max = 220;
    blue_hsv_thr.V_min = 61;

    red_hsv_thr.H_max = 15;
    red_hsv_thr.H_min = 0;
    red_hsv_thr.S_max = 255;
    red_hsv_thr.S_min = 86;
    red_hsv_thr.V_max = 255;
    red_hsv_thr.V_min = 61;

    yellow_hsv_thr.H_max = 34;
    yellow_hsv_thr.H_min = 17;
    yellow_hsv_thr.S_max = 255;
    yellow_hsv_thr.S_min = 98;
    yellow_hsv_thr.V_max = 255;
    yellow_hsv_thr.V_min = 107;

    ImageProcessing yellow_left(MEDIAN_SIZE, false, yellow_hsv_thr);
    ImageProcessing blue_left(MEDIAN_SIZE, false, blue_hsv_thr);
    RedProcess red_left(MEDIAN_SIZE, false, red_hsv_thr);

    cv::Mat source;

    while (true) {
        // std::lock_guard<std::mutex> lock(mtx);
        // printf("\nRead Thread\n");
        cap_l >> source;
        cv::resize(source, frame_l, cv::Size(), 0.5, 0.5);

        yellow_left.mainProcess(frame_l);
        blue_left.mainProcess(frame_l);
        red_left.mainProcess(frame_l);

        left_result = yellow_left.getImg(RESULT_IMAGE);
        yellow_center_l = yellow_left.getCenter();
        blue_center_l = blue_left.getCenter();
        red_center_l = red_left.getCenter();

        // cv::circle(frame_l, yellow_center_l, 10, cv::Scalar(0, 255, 255), -1, 1, 0);
        // cv::circle(frame_l, blue_center_l, 10, cv::Scalar(255, 0, 0), -1, 1, 0);
        // cv::circle(frame_l, red_center_l, 10, cv::Scalar(0, 0, 255), -1, 1, 0);

        g_leftEndFlg = 1;

        if(g_msg == 1){
            break;
        }
    }
}

int ShowFrameFromCameraFunction() {

    cv::Mat right_resize, left_resize;

    double dpi = 3.6 / 640.0;

    long totaltime_ms = 0;
    const int samplingNum = 402;
    int count = 0;
    double totaltime = 0;
    double msec;
    long totaltime_ns = 0;

    EstimatDist estimatdist(126.0, dpi, 4.0);

    // size_t nFrames = 0;
    // bool enableProcessing = false;
    // int64 t0 = cv::getTickCount();
    // int64 processingTime = 0;

    struct timespec start_vala;
    struct timespec start_val;
    while (true) {
        // std::lock_guard<std::mutex> lock(mtx);
        clock_gettime(CLOCK_MONOTONIC,&start_vala);// 消すとバグる
        if((g_rightEndFlg == 1) && (g_leftEndFlg == 1)){


            double red_dist = estimatdist.calcDist(red_center_r, red_center_l);
            double yellow_dist = estimatdist.calcDist(yellow_center_r, yellow_center_l);
            double blue_dist = estimatdist.calcDist(blue_center_r, blue_center_l);

            // cv::pyrDown(frame_r, frame_r);
            // cv::pyrDown(frame_l, frame_l);

            // printf("\nShow Thread\n");
            // cv::imshow("WIndow_r", frame_r);
            cv::circle(frame_l, yellow_center_l, 10, cv::Scalar(0, 255, 255), -1, 1, 0);
            cv::circle(frame_l, blue_center_l, 10, cv::Scalar(255, 0, 0), -1, 1, 0);
            cv::circle(frame_l, red_center_l, 10, cv::Scalar(0, 0, 255), -1, 1, 0);
            cv::imshow("Window_l", frame_l);
            // cv::imshow("WIndow_r", right_result);
            // cv::imshow("left median image", left_result);

            const int key = cv::waitKey(1);
            if (key == 27) {

                printf("\n Close Window !!\n");
                g_msg = 1;
                return 1;
            }
            else if (key == 's') {

            }

            // nFrames++;
            // // 10frameごとに表示
            // if (nFrames % 10 == 0)
            // {
            //     const int N = 10;
            //     int64 t1 = cv::getTickCount();
            //     cout << "Frames captured: " << cv::format("%5lld", (long long int)nFrames)
            //          << "    Average FPS: " << cv::format("%9.1f", (double)cv::getTickFrequency() * N / (t1 - t0))
            //          << "    Average time per frame: " << cv::format("%9.2f ms", (double)(t1 - t0) * 1000.0f / (N * cv::getTickFrequency()))
            //          << "    Average processing time: " << cv::format("%9.2f ms", (double)(processingTime) * 1000.0f / (N * cv::getTickFrequency()))
            //          << std::endl;
            //     t0 = t1;
            //     processingTime = 0;
            // }

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
                cout << "time = " << msec << "ms"<< ", yellow = " << yellow_dist << "red = " << red_dist
                << "mm, blue = " << blue_dist << "mm" << endl;

            }
            // if(count > 4){
            //     totaltime_ns += nsec;
            // }
            // if(count > samplingNum){
            //     printf("time=%ld\n", totaltime_ns);
            //     printf("time=%f\n", double(totaltime_ns) / (double)samplingNum - 4);
            //     g_msg = 1;
            //     return 1;
            //     // break;
            // }

            count++;

            g_rightEndFlg = 0;
            g_leftEndFlg = 0;
            clock_gettime(CLOCK_MONOTONIC,&start_val);

        }
    }
}


int main(int argh, char* argv[])
{

    //オープン失敗？
    if(!cap_r.isOpened())
    {
        std::cout << "ERROR: cannot open cam device." << std::endl;
        return -1;
    }

    if(!cap_l.isOpened())
    {
        std::cout << "ERROR: cannot open cam device." << std::endl;
        return -1;
    }

    cap_r.set(CV_CAP_PROP_FRAME_WIDTH, CAP_WIDTH);
    cap_r.set(CV_CAP_PROP_FRAME_HEIGHT, CAP_HEIGHT);
    // cap_r.set(CV_CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    // cap_r.set(CV_CAP_PROP_FPS, 30);

    cap_l.set(CV_CAP_PROP_FRAME_WIDTH, CAP_WIDTH);
    cap_l.set(CV_CAP_PROP_FRAME_HEIGHT, CAP_HEIGHT);
    // cap_l.set(CV_CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    // cap_l.set(CV_CAP_PROP_FPS, 30);

    std::thread GetFrameRightThread(GetFrameRightCamera);
    std::thread ShowFrameThread(ShowFrameFromCameraFunction);
    std::thread GetFrameLeftThread(GetFrameLeftCamera);

    ShowFrameThread.join();
    GetFrameRightThread.join();
    GetFrameLeftThread.join();


    cv::destroyAllWindows();
    return 0;

}
