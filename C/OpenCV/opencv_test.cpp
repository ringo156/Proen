
#include <stdio.h>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char** argv ) {

  //Mat redImg(cv::Size(320, 240), CV_8UC3, cv::Scalar(0, 0, 255));
  Mat image = imread("lena.jpg");
  int width = image.cols;
  int height = image.rows;
  //HSV
  Mat hsv_image;
  cvtColor(image, hsv_image, CV_BGR2HSV);
  uchar hue, sat, val;
  Mat mouth_image = Mat(Size(width, height), CV_8UC1);
  for (int y = 0; y < height;y++){
    for(int x = 0; x < width;x++){
      hue = hsv_image.at<Vec3b>(y, x)[0];
      sat = hsv_image.at<Vec3b>(y, x)[1];
      val = hsv_image.at<Vec3b>(y, x)[2];
      if(((hue < 8) || (hue > 168)) && (sat > 100))
        mouth_image.at<uchar>(y, x) = 0;
      else
        mouth_image.at<uchar>(y, x) = 255;
    }
  }
  // 2画像表示用のウィンドウを生成
  namedWindow("lena", WINDOW_AUTOSIZE);
  imshow("origin", image);
  // 3ウィンドウに画像を表示
  namedWindow("mouth_image");
  imshow("mouth_image", mouth_image);
  imwrite("HSV.jpg", mouth_image);
// 4キー入力を待機
  waitKey(0);
// 5作成したウィンドウを全て破棄
  destroyAllWindows();

  return 0;
}
