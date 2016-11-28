
#include <stdio.h>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/flann/miniflann.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char** argv ) {

  //Mat redImg(cv::Size(320, 240), CV_8UC3, cv::Scalar(0, 0, 255));
  Mat image = imread("lena.jpg");
// 2画像表示用のウィンドウを生成
  namedWindow("lena", WINDOW_AUTOSIZE);

// 3ウィンドウに画像を表示
  imshow("lena", image);

// 4キー入力を待機
  waitKey(0);

// 5作成したウィンドウを全て破棄
  destroyAllWindows();

  return 0;
}
