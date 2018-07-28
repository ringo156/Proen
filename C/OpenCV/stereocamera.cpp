#include <iostream>
#include <fstream>
#include <time.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Labeling.h"

#define RIGHT 2
#define LEFT  1
#define DIST_CAMERA 126.0

using namespace std;

class ImageProcessing{
    private:
        cv::Mat element;
        int median_size;
        int morpho_n;
        int morpho;

        LabelingBS label;
    public:
        int H_max, H_min, S_max, S_min, V_max, V_min;
    ImageProcessing(){
        H_max = 15;
        H_min = 0;
        S_max = 255;
        S_min = 0;
        V_max = 143;
        V_min = 36;
        cv::namedWindow("Threshold", 1);

        cv::createTrackbar("H_max", "Threshold", &H_max, 180);
        cv::createTrackbar("H_min", "Threshold", &H_min, 180);
        cv::createTrackbar("S_max", "Threshold", &S_max, 255);
        cv::createTrackbar("S_min", "Threshold", &S_min, 255);
        cv::createTrackbar("V_max", "Threshold", &V_max, 255);
        cv::createTrackbar("V_min", "Threshold", &V_min, 255);

    }

    ~ImageProcessing(){

    }
    void setting(int argMedian_size,int argMorpho_n,
        int argMorpho=cv::MORPH_ERODE,
        int argMorho_x = 3,int argMorho_y = 3){
        median_size = argMedian_size;
        morpho_n = argMorpho_n;
        morpho = argMorpho;
        element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(argMorho_y,argMorho_x));
    }
    cv::Mat filter(cv::Mat binImg){
        if(median_size != 0){
            cv::medianBlur(binImg,binImg,median_size);
        }
        for(int i;i < morpho_n;i++){
            cv::morphologyEx(binImg,binImg,morpho,element);
        }
        return binImg;
    }
    cv::Mat extruction(cv::Mat binImg){
        cv::Mat hsvImage;
        cvtColor(binImg, hsvImage, CV_BGR2HSV);
        cv::inRange(hsvImage,
            cv::Scalar(H_min, S_min, V_min, 0),
            cv::Scalar(H_max,S_max,V_max, 0),
            binImg);
        return binImg;
    }

    void labeling(cv::Mat binImg,int thSize1,int thSize2,
        cv::Point &center,cv::Point &point1,cv::Point &point2){
        cv::Mat labelImage(binImg.size(),CV_16SC1);
        label.Exec(binImg.data,(short*)labelImage.data,binImg.cols,binImg.rows,false,thSize1);
        center.x = -1;
        center.y = -1;
        for (int i = 0; i < label.GetNumOfResultRegions(); i++)
        {
            RegionInfoBS *regioninfo = label.GetResultRegionInfo(i);
            int x1, x2, y1, y2, w, h;
            regioninfo->GetMin(x1, y1);
            regioninfo->GetMax(x2, y2);
            w = x2 - x1;
            h = y2 - y1;
            float x, y;
            regioninfo->GetCenter(x, y);
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

};

int main(int argh, char* argv[])
{
    const std::string RIGHT_WIN = "RightCapture";
    const std::string LEFT_WIN  = "LeftCapture";
    const int CAP_WIDTH = 1280;
    const int CAP_HEIGHT = 960;
    double dpi = 3.6/1280.0*1.5625;
    double msec;
    cv::Mat right_result;
    cv::Mat left_result;
    cv::Point center_r, center_l, point1, point2, d;
    ImageProcessing impro;
    ofstream outputfile("test.txt");
    outputfile << "Z, d, z\n";

    impro.setting(7, 1, cv::MORPH_ERODE, 1, 3);

    //デバイスのオープン
    cv::VideoCapture right(RIGHT);
    cv::VideoCapture left(LEFT);

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

    cv::Rect rect(140, 105, 1000, 750);
    //ループ
    while(1)
    {
        struct timespec start_val;
		clock_gettime(CLOCK_MONOTONIC,&start_val);

        cv::Mat right_frame;
        cv::Mat left_frame;
        // 1フレームキャプチャ
        //cap >> frame;
        if (!right.read(right_frame))
        {
            std::cout << "ERROR: cannot capture." << std::endl;
            break;
        }
        if (!left.read(left_frame))
        {
            std::cout << "ERROR: cannot capture." << std::endl;
            break;
        }

        cv::Mat right_frame_r(right_frame, rect);
        cv::resize(right_frame_r, right_frame_r, cv::Size(), 0.64, 0.64);
        cv::Mat left_frame_r(left_frame, rect);
        cv::resize(left_frame_r, left_frame_r, cv::Size(), 0.64, 0.64);

        right_result = impro.extruction(right_frame_r);
        left_result  = impro.extruction(left_frame_r);

        cv::Mat right_red;
        cv::Mat left_red;

        cvtColor(right_frame_r, right_red, CV_BGR2HSV);
        cv::inRange(right_red,
            cv::Scalar(174, impro.S_min, impro.V_min, 0),
            cv::Scalar(180, impro.S_max, impro.V_max, 0),
            right_red);
        cvtColor(left_frame_r, left_red, CV_BGR2HSV);
        cv::inRange(left_red,
            cv::Scalar(174, impro.S_min, impro.V_min, 0),
            cv::Scalar(180, impro.S_max, impro.V_max, 0),
            left_red);

        right_result = right_result + right_red;
        left_result = left_result + left_red;

        right_result = impro.filter(right_result);
        left_result = impro.filter(left_result);
        impro.labeling(right_result,1000,20,center_r,point1,point2);
        cv::circle(right_frame_r, center_r, 10, cv::Scalar(0, 0, 0), -1, 1, 0);
        impro.labeling(left_result,1000,20,center_l,point1,point2);
        cv::circle(left_frame_r, center_l, 10, cv::Scalar(0, 0, 0), -1, 1, 0);
        d = center_l - center_r; //視差の計算

        //表示
        cv::imshow(RIGHT_WIN, right_frame_r);
        cv::imshow("right result", right_result);
        cv::imshow(LEFT_WIN, left_frame_r);
        cv::imshow("left result", left_result);

        //キー待ち
        int inp_key = cv::waitKey(1);

        double num,dis;
        dis = DIST_CAMERA * 4.0 / (dpi * d.x);
        //dis = dist * 4.0 / (dpi * d.x);
        //入力あり？
        if (inp_key >= 0) {

            if(inp_key == 'm'){
                // Z, d, z
                cin >> num;
                num = num - 80.0;
                outputfile << num << ", " << d.x << ", " << dis <<endl;
            }
            else if (inp_key == 27 || inp_key == 81 || inp_key == 113) break;
        }
        struct timespec end;
		clock_gettime(CLOCK_MONOTONIC,&end);
		long sec = end.tv_sec - start_val.tv_sec;
		long nsec = end.tv_nsec-start_val.tv_nsec;
		if(nsec < 0){
			sec--;
			nsec += 1000000000L;
		}
		msec = sec*1000+nsec/1000000;

        cout << "time = " << msec << "ms, d.x = " << d.x << "pixel, " << "z = " << dis << "mm" << endl;
    }
    //ウィンドウ破棄
    cv::destroyAllWindows();
    outputfile.close();
    return 0;
}
