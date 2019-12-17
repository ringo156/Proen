// opencv2 for multithread
//

#include <time.h>
#include <iostream>
#include <stdio.h>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "stereocamera.hpp"

#define RIGHT 1
#define LEFT  2

#define CAP_WIDTH  1280
#define CAP_HEIGHT  960

int init_stereocam(cv::VideoCapture right, cv::VideoCapture left);

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


int main(int argc, char **argv){

    const std::string RIGHT_WIN = "RightCapture";
    const std::string LEFT_WIN  = "LeftCapture";
    const int median_size = 7;

    long totaltime_ms = 0;
    const int samplingNum = 402;
    int count = 0;
    double totaltime = 0;
    double msec;
    long totaltime_ns = 0;

    double dpi = 3.6 / 640.0;

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

    ImageProcessing blue_left(median_size, true, blue_hsv_thr);
    ImageProcessing blue_right(median_size, false, blue_hsv_thr);
    ImageProcessing yellow_left(median_size, false, yellow_hsv_thr);
    ImageProcessing yellow_right(median_size, false, yellow_hsv_thr);
    RedProcess red_left(median_size, false, red_hsv_thr);
    RedProcess red_right(median_size, false, red_hsv_thr);

    cout << "Opening camera..." << endl;
    cv::VideoCapture right_cap(RIGHT); // open the first camera
    cv::VideoCapture left_cap(LEFT); // open the first camera

    init_stereocam(right_cap, left_cap);

    size_t nFrames = 0;
    bool enableProcessing = false;
    int64 processingTime = 0;

    cv::Mat right_frame;
    cv::Mat left_frame;
    cv::Mat left_resize, right_resize;

    cv::Point red_center_r, red_center_l, blue_center_r, blue_center_l;
    cv::Point yellow_center_r, yellow_center_l;

    EstimatDist estimatdist(126.0, dpi, 4.0);

    int64 t0 = cv::getTickCount();
    int64 tp0 = cv::getTickCount();

    for(;;){
        struct timespec start_val;
        clock_gettime(CLOCK_MONOTONIC,&start_val);

        right_cap >> right_frame;
        left_cap >> left_frame;

        cv::resize(right_frame, right_resize, cv::Size(), 0.5, 0.5);
        cv::resize(left_frame, left_resize, cv::Size(), 0.5, 0.5);

        blue_right.mainProcess(right_resize);
        blue_left.mainProcess(left_resize);
        yellow_right.mainProcess(right_resize);
        yellow_left.mainProcess(left_resize);
        red_right.mainProcess(right_resize);
        red_left.mainProcess(left_resize);
        //
        blue_center_r = blue_right.getCenter();
        blue_center_l = blue_left.getCenter();
        yellow_center_r = yellow_right.getCenter();
        yellow_center_l = yellow_left.getCenter();
        red_center_r = red_right.getCenter();
        red_center_l = red_left.getCenter();

        double red_dist = estimatdist.calcDist(red_center_r, red_center_l);
        double yellow_dist = estimatdist.calcDist(yellow_center_r, yellow_center_l);
        double blue_dist = estimatdist.calcDist(blue_center_r, blue_center_l);

        // cv::imshow("median img", red_left.getImg(RESULT_IMAGE));
        // cv::imshow("binary img", red_left.getImg(BINARY_IMAGE));
        cv::circle(left_resize, yellow_center_l, 10, cv::Scalar(0, 255, 255), -1, 1, 0);
        cv::circle(right_resize, yellow_center_r, 10, cv::Scalar(0, 255, 255), -1, 1, 0);
        cv::circle(left_resize, blue_center_l, 10, cv::Scalar(255, 0, 0), -1, 1, 0);
        cv::circle(left_resize, red_center_l, 10, cv::Scalar(0, 0, 255), -1, 1, 0);

        // cv::imshow("red median img", red_left.getImg(RESULT_IMAGE));
        // cv::imshow("blue median img", blue_right.getImg(RESULT_IMAGE));
        // cv::imshow("right result img", right_resize);
        cv::imshow("left result img", left_resize);

        int key = cv::waitKey(1); // too late!
        // processingTime += cv::getTickCount() - tp0;

        // nFrames++;
        // 10frameごとに表示
        // if (nFrames % 10 == 0)
        // {
        //     const int N = 10;
        //     int64 t1 = cv::getTickCount();
        //     cout << "Frames captured: " << cv::format("%5lld", (long long int)nFrames)
        //          << "    Average FPS: " << cv::format("%9.1f", (double)cv::getTickFrequency() * N / (t1 - t0))
        //          << "    Average time per frame: " << cv::format("%9.2f ms", (double)(t1 - t0) * 1000.0f / (N * cv::getTickFrequency()))
        //          << "    Average processing time: " << cv::format("%9.2f ms", (double)(processingTime) * 1000.0f / (N * cv::getTickFrequency()))
        //          << std::endl;
        //                  cout << "x=" << red_center_l.x << ", y=" << red_center_l.y << ", dist = "<< dist << endl;
        //
        //     if(nFrames > 4){
        //          totaltime_ms += (t1 - t0);
        //     }
        //     if(nFrames > samplingNum){
        //          printf("time=%ld\n", totaltime_ms);
        //          printf("time=%f\n", double(totaltime_ms) / (double)samplingNum - 4);
        //          break;
        //     }
        //
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
            cout << "time = " << msec << "ms, "<< "red = " << red_dist << "mm, blue = "
            << blue_dist << "mm, yellow = "<< yellow_dist << "mm" << endl;
            //", yellow = " << yellow_dist << endl;
            // ", blue = " << blue_dist <<endl;

        }

        // if(count > 4){
        //     totaltime_ns += nsec;
        // }
        // if(count > samplingNum){
        //     printf("time=%ld\n", totaltime_ns);
        //     printf("time=%f\n", double(totaltime_ns) / (double)samplingNum - 4);
        //     break;
        // }

        if(key == 27){
            break;
        }
        if(key == 32){
            enableProcessing = !enableProcessing;
            cout << "Enable frame processing ('space' key): " << enableProcessing << endl;
        }
        count++;
    }

    cv::destroyAllWindows();
    return 0;

}


int init_stereocam(cv::VideoCapture right, cv::VideoCapture left){
    if (!right.isOpened())
    {
        cerr << "ERROR: Can't initialize right camera capture" << endl;
        return 1;
    }
    // change cordec

    right.set(CV_CAP_PROP_FRAME_WIDTH, CAP_WIDTH);
    right.set(CV_CAP_PROP_FRAME_HEIGHT, CAP_HEIGHT);
    // right.set(CV_CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    // right.set(CV_CAP_PROP_FPS, 30);

    if (!left.isOpened())
    {
        cerr << "ERROR: Can't initialize left camera capture" << endl;
        return 1;
    }

    left.set(CV_CAP_PROP_FRAME_WIDTH, CAP_WIDTH);
    left.set(CV_CAP_PROP_FRAME_HEIGHT, CAP_HEIGHT);
    // left.set(CV_CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    // left.set(CV_CAP_PROP_FPS, 30);

    return 0;
}
