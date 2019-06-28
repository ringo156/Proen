#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Labeling.h"

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
        S_min = 122;
        V_max = 224;
        V_min = 66;
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

    void median_one_ptr(const cv::Mat& src, cv::Mat& dist){
        dist = src.clone();
        uchar* ptr = dist.ptr<unsigned char>(0);
        int buf = 0;
        // for(int y = 0; y < src.rows * src.cols; y = y + src.cols){
        //     for(int x = 10; x < src.cols - 11; x++){
        //         buf = 0;
        //         for(int i = 0; i < 11; i++){
        //             if((int)ptr[y + x + i - 4] == 0){ // 0 is black
        //                 buf++;
        //             }
        //             if(buf == 6){
        //                 ptr[y + x - 4] = 0;
        //                 break;
        //             }
        //             else{
        //                 ptr[y + x - 4] = 255;
        //             }
        //         }
        //     }
        // }
        for(int y = 0; y < src.rows * src.cols; y = y + src.cols){
            for(int x = 14; x < src.cols - 15; x++){
                buf = 0;
                for(int i = 0; i < 15; i++){
                    if((int)ptr[y + x + i - 7] == 0){ // 0 is black
                        buf++;
                    }
                    if(buf == 9){
                        ptr[y + x - 7] = 0;
                        break;
                    }
                    else{
                        ptr[y + x - 7] = 255;
                    }
                }
            }
        }
        ptr = dist.ptr<unsigned char>(0);
        for(int x = 0; x < src.cols; x = ++x){
            for(int y = 14; y < src.rows - 15; y++){
                buf = 0;
                for(int i = 0; i < 15; i++){
                    if((int)ptr[(y * src.cols) + x + (i * src.cols) - (7 * src.cols) ] == 0){// here
                        buf++;
                    }
                    if(buf == 9){
                        ptr[(y * src.cols) + x - (7 * src.cols)] = 0;
                        break;
                    }
                    else{
                        ptr[(y * src.cols) + x - (7 * src.cols)] = 255;
                    }
                }
            }
        }
    }

};
