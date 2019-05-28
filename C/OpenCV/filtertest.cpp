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

    Mat binary_image;
    // extruction for HSV
    inRange(hsv_image,
        Scalar(0, 0, 0, 0),
        Scalar(15, 255, 255, 0),
        binary_image
    );

    Mat median_test = binary_image.clone();
    //Mat median_test(binary_image.rows, binary_image.cols, CV_8UC1);
    //uchar* ptr = median_test.ptr<uchar>(0);

    // for(int j = 0; j < median_test.rows; j++){
    //     for(int i = 0; i < median_test.cols; i++){
    //         cout << (int)median_test.at<unsigned char>(j, i) << endl; // 255 is white
    //     }
    // }

    int buf = 0;
    for(int y = 0; y < median_test.rows;y++){
        for(int x = 15; x < median_test.cols - 15; x++){
            buf = 0;
            for(int i = 0; i < 15; i++){
                if((int)median_test.at<unsigned char>(y, (x + i - 7)) == 0){
                    buf++;
                }
                if(buf == 9){
                    median_test.at<unsigned char>(y, x - 7) = 0;
                    break;
                }
                else{
                    median_test.at<unsigned char>(y, x - 7) = 255;
                }
            }
        }
    }

/*
    for(int y = 0; y < median_test.rows * median_test.cols ; y = y + median_test.cols){
        for(int x = 0; x < median_test.cols - 40; ++x){
            for(int i = 0; i < 15; ++i){
                int buf = 0;
                // 画素値にアクセスして加算する
                //Vec3b* ptr = median_test.ptr<Vec3b>(y); // バグる（笑）
                //uchar* ptr = median_test.ptr<uchar>(y);
                //buf += ptr[i + x];

                buf += (int)median_test.at<unsigned char>(x + i, y);

                if(buf >= 2295){
                    //画素を黒にする
                    median_test.at<unsigned char>(x) = 255;
                    break;
                }
            }
        }
    }
    */


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

void median_one(const Mat& src, Mat& dist, const int size){

}
