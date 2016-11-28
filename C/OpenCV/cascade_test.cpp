#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
 
#ifdef _DEBUG
#pragma comment(lib,"C:\\OpenCV\\build\\x64\\vc14\\lib\\opencv_world310d.lib")
#else
#pragma comment(lib,"C:\\OpenCV\\build\\x64\\vc14\\lib\\opencv_world310.lib")
#endif
 
using namespace cv;
using namespace std;
 
Mat detectFaceInImage(Mat &image, string &cascade_file)
{
    CascadeClassifier cascade;
    cascade.load(cascade_file);
 
    vector<Rect> faces;
    cascade.detectMultiScale(image, faces, 1.1, 3, 0, Size(20, 20));
 
    for (int i = 0; i < faces.size(); i++) {
        rectangle(image, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 200, 0), 3, CV_AA);
    }
    return image;
}
 
int main(int argc, char const *argv[])
{
    Mat image = imread("lena.jpg");
    string filename = "C:\\OpenCV\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml";
    Mat detectFaceImage = detectFaceInImage(image, filename);
    imshow("detect face", detectFaceImage);
    waitKey(0);
 
    return 0;
}
