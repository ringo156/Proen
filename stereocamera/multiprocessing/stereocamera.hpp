
#ifndef __STEREOCAMERA_HPP__
#define __STEREOCAMERA_HPP__

#include <iostream>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// #include <opencv2/opencv.hpp>
// #include <opencv2/core.hpp>
// #include <opencv2/videoio.hpp>
// #include <opencv2/highgui.hpp>
// #include <opencv2/imgproc.hpp>

#include "./../test/Labeling.h"

#define SOURCE_IMAGE 1
#define BINARY_IMAGE 2
#define RESULT_IMAGE 3

using namespace std;

struct HSV_threshold{
    int H_max;
    int H_min;
    int S_max;
    int S_min;
    int V_max;
    int V_min;
};

// image process class
class ImageProcessing{
private:
    int kernel_size;
    cv::Mat org;
    cv::Mat binary_img;
    cv::Mat result;
    cv::Mat element;

    cv::Point center;

    LabelingBS label;

public:

    int H_max, H_min, S_max, S_min, V_max, V_min;
    ImageProcessing(const int _ksize, const bool _trackBar, const HSV_threshold _thr)
    : kernel_size(_ksize), H_max(_thr.H_max), H_min(_thr.H_min), S_max(_thr.S_max),
    S_min(_thr.S_min), V_max(_thr.V_max), V_min(_thr.V_min){

        if(_trackBar == true){
            cv::namedWindow("Threshold", 1);

            cv::createTrackbar("H_max", "Threshold", &H_max, 180);
            cv::createTrackbar("H_min", "Threshold", &H_min, 180);
            cv::createTrackbar("S_max", "Threshold", &S_max, 255);
            cv::createTrackbar("S_min", "Threshold", &S_min, 255);
            cv::createTrackbar("V_max", "Threshold", &V_max, 255);
            cv::createTrackbar("V_min", "Threshold", &V_min, 255);
        }
    }


    ~ImageProcessing(){
    }

    // いい感じの書き方が分からん
    // OpenCVのcodeを参考にして
    void mainProcess(const cv::Mat& _org_img){
        org = _org_img;

        colorExtruction(org, binary_img);
        cv::medianBlur(binary_img, result, kernel_size);
        // labeling
        // 今までのやつとOpenCV3ので比較する
        // 一応比較してる文献あるか調べる
        cv::Point point1, point2;
        labeling(binary_img, 1000, 20, center, point1, point2);

    }

    virtual void colorExtruction(cv::Mat& src, cv::Mat& dist){
        cv::cvtColor(src, dist, CV_BGR2HSV);

        cv::inRange(dist,
            cv::Scalar(H_min, S_min, V_min, 0),
            cv::Scalar(H_max, S_max, V_max, 0),
            dist);

    }

    void labeling(cv::Mat binImg, int thSize1, int thSize2,
        cv::Point &center, cv::Point &point1, cv::Point &point2){
        cv::Mat labelImage(binImg.size(),CV_16SC1);

        label.Exec(binImg.data, (short*)labelImage.data, binImg.cols, binImg.rows, false, thSize1);

        center.x = -1;
        center.y = -1;

        for (int i = 0; i < label.GetNumOfResultRegions(); ++i)
        {
            RegionInfoBS *regioninfo = label.GetResultRegionInfo(i);
            int x1, x2, y1, y2, w, h;
            regioninfo -> GetMin(x1, y1);
            regioninfo -> GetMax(x2, y2);
            w = x2 - x1;
            h = y2 - y1;
            float x, y;
            regioninfo -> GetCenter(x, y);
            int center_x = (int)x;
            int center_y = (int)y;

            if((w > thSize2) && (h > thSize2)){
                point1.x = x1;
                point1.y = y1;
                point2.x = x2;
                point2.y = y2;
                center.x = center_x;
                center.y = center_y;
                return;
            }
        }
    }

    // 返すのはポインタ？コピー？？
    cv::Mat getImg(const int name){
        if(name == SOURCE_IMAGE){
            return org;
        }else
        if(name == BINARY_IMAGE){
            return binary_img;
        }else
        if(name == RESULT_IMAGE){
            return result;
        }
    }
    cv::Point getCenter(){
        return center;
    }
    void setKsize(const int _ksize){
        kernel_size = _ksize;
    }
    int getKsize(){
        return kernel_size;
    }

};

// distance estimation for stereo image
class EstimatDist{
    private:
        cv::Point center_r, center_l, parallax;
        double dist_camera, dpi, dist, fl; // focal length

    public:

        EstimatDist(double _dist_camera, double _dpi, double _fl)
        : dist_camera(_dist_camera), dpi(_dpi), fl(_fl){
        }
        ~EstimatDist(){
        }

        double calcDist(cv::Point const &_center_r, cv::Point const &_center_l){
            parallax = _center_l - _center_r;

            dist = dist_camera * fl / (dpi * parallax.x);

            return dist;
        }

        cv::Point getParallax(){
            return parallax;
        }

        // まだ隠せると思う
        void setDPI(double _dpi){
            dpi = _dpi;
        }

};

#endif // __STEREOCAMERA_HPP__
