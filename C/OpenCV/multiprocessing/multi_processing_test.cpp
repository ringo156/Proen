#include <iostream>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
// #include "opencv2/imgproc/imgproc.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    CvCapture* capture = 0;
    const std::string WIN_NAME = "Capture";
    const int CAP_WIDTH = 1280;
    const int CAP_HEIGHT = 960;
    double msec;
//  const std::string SAVE_FILE = "Img.jpg";

    cout << "device open"<< endl;
    //デバイスのオープン
    cv::VideoCapture cap(0);

    cap.open(0);

    // オープン失敗？
    if(!cap.isOpened())
    {
        std::cout << "ERROR: cannot open cam device." << std::endl;
        return -1;
    }

    // キャプチャサイズの設定
    cap.set(CV_CAP_PROP_FRAME_WIDTH, CAP_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAP_HEIGHT);
    //ループ
    while(1)
    {
        struct timespec start_val;
		clock_gettime(CLOCK_REALTIME,&start_val);

        cv::Mat frame;
        // 1フレームキャプチャ
        cap >> frame;
        if (!cap.read(frame))
        {
            std::cout << "ERROR: cannot capture." << std::endl;
            break;
        }

        // cv::Mat gray;
        // cv::cvtColor(frame, gray, CV_BGR2GRAY);

        cv::Mat gray(frame.rows, frame.cols, CV_8UC1);
        cv::Vec3b* ptr = frame.ptr<cv::Vec3b>(0);
        uchar* g_ptr = gray.ptr<uchar>(0);
        int x = 0;
        #pragma omp parallel for private(x)
        for(int y = 0; y < frame.rows * frame.cols; y = y + frame.cols){
            for(x = 0; x < frame.cols; ++x){
                cv::Vec3b bgr = ptr[x + y];
                //cout << "B:" << (int)bgr[0] << ", G" << (int)bgr[1] << ", R:" <<(int)bgr[2] << endl;
                g_ptr[x + y] = (unsigned char)(0.114 * bgr[0] + 0.587 * bgr[1] + 0.299 * bgr[2]);
            }
        }

        // cvtColor(frame, dist, CV_BGR2HSV);
        // cv::inRange(dist, cv::Scalar(0, 0, 0, 0), cv::Scalar(10, 255, 255, 0), dist);
        // cv::medianBlur(dist, dist, 7);

        //キー待ち
        int inp_key = cv::waitKey(1);
        //入力あり？
        if (inp_key >= 0) {
            // [ESC] or 'Q' or 'q'
            if (inp_key == 27 || inp_key == 81 || inp_key == 113) break;

        }
        struct timespec end;
		clock_gettime(CLOCK_REALTIME,&end);

        printf("elapsed time = ");
        if (end.tv_nsec < start_val.tv_nsec) {
          printf("%5ld.%09ld", end.tv_sec - start_val.tv_sec - 1,
                 end.tv_nsec + (long int)1.0e+9 - start_val.tv_nsec);
        } else {
          printf("%5ld.%09ld", end.tv_sec - start_val.tv_sec,
                 end.tv_nsec - start_val.tv_nsec);
        }
        printf("(sec)\n");

        //表示
        cv::imshow(WIN_NAME, frame);
        cv::imshow("result", gray);

    } //end of while

    //ウィンドウ破棄
    cv::destroyAllWindows();

    return 0;
}
