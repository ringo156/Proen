#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace cv;
using namespace std;

Mat detectFaceInImage(Mat &image, string &cascade_file);
string intToString(int val);

int main(int argc, char const *argv[])
{
    Mat image = imread("image-18.jpg");
    string filename = "/home/ringo/prog/opencv-2.4.13/data/haarcascades/haarcascade_frontalface_alt.xml";
    Mat detectFaceImage = detectFaceInImage(image, filename);
    imshow("detect face", detectFaceImage);
    waitKey(0);

    return 0;
}

string intToString(int val)
{
  stringstream ss;
  ss << val;
  return ss.str();
}

Mat detectFaceInImage(Mat &image, string &cascade_file)
{
    CascadeClassifier cascade;
    cascade.load(cascade_file);

    vector<Rect> faces;
    cascade.detectMultiScale(image, faces, 1.1, 3, 0, Size(20, 20));
    /*
    cout << "x=" << faces[0].x << endl;
    cout << "y=" << faces[0].y << endl;
    cout << "width=" << faces[0].width << endl;
    cout << "height=" << faces[0].height << endl;
    */
    //顔画像の切り抜き
    for (int i=0; i < faces.size(); i++){
      Mat cut_img(image, cv::Rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height));
      string str = intToString(i);//名前付ける
      imshow(str, cut_img);
      string jpg = ".jpg";
      str = str + jpg;
      imwrite (str, cut_img);
    }
    for (int i = 0; i < faces.size(); i++) {
        rectangle(image, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 200, 0), 3, CV_AA);
    }
    imwrite("result.jpg", image);
    //imshow("cut_face", cut_img);
    return image;
}
