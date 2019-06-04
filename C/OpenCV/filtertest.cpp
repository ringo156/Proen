#include <stdio.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void median_one(const cv::Mat& src, cv::Mat& dist);
void median_one_ptr(const cv::Mat& src, cv::Mat& dist);

int main(int argc, const char** argv ) {

    //Mat redImg(cv::Size(320, 240), CV_8UC3, cv::Scalar(0, 0, 255));
    Mat image = imread("lena.jpg");
    int width = image.cols;
    int height = image.rows;

    //HSV conversion---------
    Mat hsv_image;
    cvtColor(image, hsv_image, CV_BGR2HSV);

    Mat binary_image;
    // extruction for HSV---------
    inRange(hsv_image,
        Scalar(0, 0, 0, 0),
        Scalar(15, 255, 255, 0),
        binary_image
    );

    Mat median_test;
    // median filter of one dimension for pointer access
    median_one_ptr(binary_image, median_test);
    
    // median conversion for OpenCV medianBlur
    Mat median_image;
    medianBlur(binary_image, median_image, 7);

    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat morpho_image;
    morphologyEx(binary_image, morpho_image, MORPH_CLOSE, element);

    // 2画像表示用のウィンドウを生成
    namedWindow("origin", WINDOW_AUTOSIZE);
    imshow("origin", image);
    // 3ウィンドウに画像を表示
    namedWindow("binary_image");
    imshow("binary_image", binary_image);
    imshow("median_image", median_image);
    imshow("morpho_image", morpho_image);
    imshow("median_test", median_test);
    imwrite("median_test.jpg", median_test);
    //imwrite("median.jpg", median_image);
    // 4キー入力を待機
    waitKey(0);
    // 5作成したウィンドウを全て破棄
    destroyAllWindows();

    return 0;
}


void median_one(const cv::Mat& src, cv::Mat& dist, const int size){
    dist = src.clone();
    int buf = 0;
    for(int y = 0; y < src.rows;y++){
        for(int x = 14; x < src.cols - 15; x++){
            buf = 0;
            for(int i = 0; i < 15; i++){
                if((int)src.at<unsigned char>(y, (x + i - 7)) == 0){ // 0 is black
                    buf++;
                }
                if(buf == 9){
                    dist.at<unsigned char>(y, x - 7) = 0;
                    break;
                }
                else{
                    dist.at<unsigned char>(y, x - 7) = 255;
                }
            }
        }
    }
}

void median_one_ptr(const cv::Mat& src, cv::Mat& dist){
    dist = src.clone();
    uchar* ptr = dist.ptr<unsigned char>(0);
    int buf = 0;
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
}
