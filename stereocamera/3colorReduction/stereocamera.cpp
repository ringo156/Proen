// migrate opencv2 to opencv3 test program
//

#include <time.h>
#include <iostream>

#include "opencv2/core/version.hpp"

#if CV_MAJOR_VERSION == 2
// do opencv 2 code
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"


#elif CV_MAJOR_VERSION == 3
// do opencv 3 code
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#endif

#include "stereocamera.hpp"

#define RIGHT 2
#define LEFT  1

#define CAP_WIDTH  1280
#define CAP_HEIGHT  960

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

    double dpi = 3.6 / 640.0;

    HSV_threshold hsv_thr;

    hsv_thr.H_max = 81;
    hsv_thr.H_min = 47;
    hsv_thr.S_max = 255;
    hsv_thr.S_min = 86;
    hsv_thr.V_max = 220;
    hsv_thr.V_min = 61;

    ImageProcessing blue_left(7, false, hsv_thr);
    ImageProcessing blue_right(7, false, hsv_thr);
    ImageProcessing yellow_left(7, false, hsv_thr);
    ImageProcessing yellow_right(7, false, hsv_thr);
    RedProcess red_left(7, true, hsv_thr);
    RedProcess red_right(7, false, hsv_thr);


    cout << "Opening camera..." << endl;
    cv::VideoCapture right_cap(RIGHT); // open the first camera
    cv::VideoCapture left_cap(LEFT); // open the first camera

    init_stereocam(right_cap, left_cap);

    size_t nFrames = 0;
    bool enableProcessing = false;
    int64 t0 = cv::getTickCount();
    int64 processingTime = 0;

    cv::Mat right_frame;
    cv::Mat left_frame;
    cv::Mat left_resize, right_resize;

    cv::Point red_center_r, red_center_l, blue_center_r, blue_center_l;
    cv::Point yellow_center_r, yellow_center_l;

    EstimatDist estimatdist(126.0, dpi, 4.0);

    int64 tp0 = cv::getTickCount();

    for(;;){
        struct timespec start_val;
        clock_gettime(CLOCK_MONOTONIC,&start_val);

        right_cap >> right_frame;
        left_cap >> left_frame;

        cv::resize(right_frame, right_resize, cv::Size(), 0.5, 0.5);
        cv::resize(left_frame, left_resize, cv::Size(), 0.5, 0.5);

        // blue_right.mainProcess(right_resize);
        // blue_left.mainProcess(left_resize);
        yellow_right.mainProcess(right_resize);
        yellow_left.mainProcess(left_resize);
        red_right.mainProcess(right_resize);
        red_left.mainProcess(left_resize);

        // blue_center_r = blue_right.getCenter();
        // blue_center_l = blue_left.getCenter();
        yellow_center_r = yellow_right.getCenter();
        yellow_center_l = yellow_left.getCenter();
        red_center_r = red_right.getCenter();
        red_center_l = red_left.getCenter();

        double dist = estimatdist.calcDist(yellow_center_r, yellow_center_l);


        // cv::imshow("median img", left.getImg(RESULT_IMAGE));
        cv::circle(left_resize, yellow_center_l, 10, cv::Scalar(0, 255, 0), -1, 1, 0);
        // cv::imshow("org img", left_resize);

        cv::imshow("red median img", red_left.getImg(RESULT_IMAGE));
        cv::circle(left_resize, red_center_l, 10, cv::Scalar(0, 0, 255), -1, 1, 0);
        cv::imshow("org img", left_resize);

        int key = cv::waitKey(1); // too late!
        processingTime += cv::getTickCount() - tp0;

        nFrames++;
        // 10frameごとに表示
        if (nFrames % 10 == 0)
        {
            const int N = 10;
            int64 t1 = cv::getTickCount();
            cout << "Frames captured: " << cv::format("%5lld", (long long int)nFrames)
                 << "    Average FPS: " << cv::format("%9.1f", (double)cv::getTickFrequency() * N / (t1 - t0))
                 << "    Average time per frame: " << cv::format("%9.2f ms", (double)(t1 - t0) * 1000.0f / (N * cv::getTickFrequency()))
                 << "    Average processing time: " << cv::format("%9.2f ms", (double)(processingTime) * 1000.0f / (N * cv::getTickFrequency()))
                 << std::endl;
                         cout << "x=" << red_center_l.x << ", y=" << red_center_l.y << ", dist = "<< dist << endl;
            t0 = t1;
            processingTime = 0;
        }

        if(key == 27){
            break;
        }
        if(key == 32){
            enableProcessing = !enableProcessing;
            cout << "Enable frame processing ('space' key): " << enableProcessing << endl;
        }
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
