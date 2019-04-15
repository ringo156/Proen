#include <iostream>
#include <stdio.h>
#include <time.h>

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

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

    } //end of while

    //ウィンドウ破棄
    cv::destroyAllWindows();

    return 0;
}
