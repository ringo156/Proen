#include <stdio.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void median_one(const Mat& src, Mat& dist, const int size);

int main(int argc, const char** argv ) {

    //Mat redImg(cv::Size(320, 240), CV_8UC3, cv::Scalar(0, 0, 255));
    Mat image = imread("lena.jpg");
    int width = image.cols;
    int height = image.rows;
    //HSV
    Mat hsv_image;
    cvtColor(image, hsv_image, CV_BGR2HSV);

    Mat result_image;
    // extruction for HSV
    inRange(hsv_image,
        Scalar(0, 0, 0, 0),
        Scalar(15, 255, 255, 0),
        result_image
    );

    Mat median_image;
    medianBlur(result_image, median_image, 7);

    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat morpho_image;
    morphologyEx(result_image, morpho_image, MORPH_CLOSE, element);

    // 2画像表示用のウィンドウを生成
    namedWindow("origin", WINDOW_AUTOSIZE);
    imshow("origin", image);
    // 3ウィンドウに画像を表示
    namedWindow("result_image");
    imshow("result_image", result_image);
    imshow("median_image", median_image);
    imshow("morpho_image", morpho_image);
    imwrite("median.jpg", median_image);
    // 4キー入力を待機
    waitKey(0);
    // 5作成したウィンドウを全て破棄
    destroyAllWindows();

    return 0;
}

void median_one(const Mat& src, Mat& dist, const int size){
    
}
