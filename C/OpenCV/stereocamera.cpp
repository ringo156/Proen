#include <iostream>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Labeling.h"

#define RIGHT 2
#define LEFT  1

using namespace std;

class ImageProcessing{
    private:
        cv::Mat element;
        int median_size;
        int morpho_n;
        int morpho;
        //int H_max, H_min, S_max, S_min, V_max, V_min;
        LabelingBS label;
    public:
    ImageProcessing(){
        /*
        H_max = 15;
        H_min = 0;
        S_max = 255;
        S_min = 0;
        V_max = 143;
        V_min = 36;
        cv::namedWindow("Threshold", 1);
        
        cv::createTrackbar("H_max", "Threshold", &H_max, 255);
        cv::createTrackbar("H_min", "Threshold", &H_min, 255);
        cv::createTrackbar("S_max", "Threshold", &S_max, 255);
        cv::createTrackbar("S_min", "Threshold", &S_min, 255);
        cv::createTrackbar("V_max", "Threshold", &V_max, 255);
        cv::createTrackbar("V_min", "Threshold", &V_min, 255);
        */
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
/*
    void extraction(cv::Mat* src, cv::Mat* dst,int code)
    {
        cv::Mat colorImage;
        int lower[3];
        int upper[3];
        int ch1Lower, ch1Upper, ch2Lower, ch2Upper;
        int ch3Lower, ch3Upper;
        ch1Lower = H_min;
        ch1Upper = H_max;
        ch2Lower = S_min;
        ch2Upper = S_max;
        ch3Lower = V_min;
        ch3Upper = V_max;

        cv::Mat lut = cv::Mat(256, 1, CV_8UC3);   

        cv::cvtColor(*src, colorImage, code);

        lower[0] = ch1Lower;
        lower[1] = ch2Lower;
        lower[2] = ch3Lower;

        upper[0] = ch1Upper;
        upper[1] = ch2Upper;
        upper[2] = ch3Upper;

        for (int i = 0; i < 256; i++){
            for (int k = 0; k < 3; k++){
                if (lower[k] <= upper[k]){
                    if ((lower[k] <= i) && (i <= upper[k])){
                        lut.data[i*lut.step+k] = 255;
                    }else{
                        lut.data[i*lut.step+k] = 0;
                    }
                }else{
                    if ((i <= upper[k]) || (lower[k] <= i)){
                        lut.data[i*lut.step+k] = 255;
                    }else{
                        lut.data[i*lut.step+k] = 0;
                    }
                }
            }
        }

        //LUTを使用して二値化
        cv::LUT(colorImage, lut, colorImage);

        //Channel毎に分解
        std::vector<cv::Mat> planes;
        cv::split(colorImage, planes);

        //マスクを作成
        cv::Mat maskImage;
        cv::bitwise_and(planes[0], planes[1], maskImage);
        cv::bitwise_and(maskImage, planes[2], maskImage);

        //出力
        cv::Mat maskedImage;
        src->copyTo(maskedImage, maskImage);
        *dst = maskImage;
    }
    */

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
    const int CAP_WIDTH = 640;
    const int CAP_HEIGHT = 480;
    cv::Mat right_result;
    cv::Mat left_result;
    cv::Point center_r, center_l, point1, point2, d;
    ImageProcessing impro;
    ofstream outputfile("test.txt");
    outputfile << "z, d, zz, x, xtes\n";
    
    //Create Trackbar from HSV threshold
    int H_max, H_min, S_max, S_min, V_max, V_min;
    
        H_max = 165;
        H_min = 150;
        S_max = 255;
        S_min = 100;
        V_max = 255;
        V_min = 70;
    
        cv::namedWindow("Threshold", 1);
      
        cv::createTrackbar("H_max", "Threshold", &H_max, 180);
        cv::createTrackbar("H_min", "Threshold", &H_min, 180);
        cv::createTrackbar("S_max", "Threshold", &S_max, 255);
        cv::createTrackbar("S_min", "Threshold", &S_min, 255);
        cv::createTrackbar("V_max", "Threshold", &V_max, 255);
        cv::createTrackbar("V_min", "Threshold", &V_min, 255);

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

    //ループ
    while(1)
    {
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

        cvtColor(right_frame, right_result, CV_BGR2HSV);
        cvtColor(left_frame, left_result, CV_BGR2HSV);

        cv::inRange(right_result,
            cv::Scalar(H_min, S_min, V_min, 0),
            cv::Scalar(H_max,S_max,V_max, 0),
            right_result);
        cv::inRange(left_result,
            cv::Scalar(H_min, S_min, V_min, 0),
            cv::Scalar(H_max,S_max,V_max, 0),
            left_result);

        right_result = impro.filter(right_result);
        left_result = impro.filter(left_result);  
        impro.labeling(right_result,1000,20,center_r,point1,point2);
        cv::circle(right_frame, center_r, 10, cv::Scalar(0, 0, 0), -1, 1, 0);
        impro.labeling(left_result,1000,20,center_l,point1,point2);
        cv::circle(left_frame, center_l, 10, cv::Scalar(0, 0, 0), -1, 1, 0);  
        d = center_l - center_r;

        double z, x, f;
        if(d.x == 0){
            z = 0;
        }
        else{
           // z = 4.0 * 96.0 / (double)d.x;
            z = (81296.64 - 10.8672 * (double)d.x) / (double)d.x;
        }
        cout << "d.x = " << d.x << "pixel,z = " << z << "mm" << endl;

        //表示
        cv::imshow(RIGHT_WIN, right_frame);
        cv::imshow("right result", right_result);
        cv::imshow(LEFT_WIN, left_frame);
        cv::imshow("left result", left_result);

        //キー待ち
        int inp_key = cv::waitKey(1);

        //入力あり？
        if (inp_key >= 0) {
            if(inp_key == 'm'){
                double num;
                cin >> num;
                num = num - 300.0;
                x = num * d.x / 96.0;
                f = num / z;
                outputfile << num << ", " << d.x << ", " << z << ", " 
                << x <<", "<< f << endl;
            }
            else if (inp_key == 27 || inp_key == 81 || inp_key == 113) break;
        }
    } 
    //ウィンドウ破棄
    cv::destroyAllWindows();
    outputfile.close();
    return 0;
}
