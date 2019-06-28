#include <iostream>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;

void median_one(const cv::Mat& src, cv::Mat& dist, const int size);
void median_one_ptr(const cv::Mat& src, cv::Mat& dist);

int main(int argc, char* argv[])
{
    const std::string WIN_NAME = "Capture";
    const int CAP_WIDTH  = 1280;
    const int CAP_HEIGHT = 960;
    const int samplingNum = 402;
    int count = 0;
    double totaltime = 0;
    double msec;
    long totaltime_ns = 0;
//  const std::string SAVE_FILE = "Img.jpg";

    int H_max, H_min, S_max, S_min, V_max, V_min;
    H_max = 15;
    H_min = 0;
    S_max = 255;
    S_min = 122;
    V_max = 224;
    V_min = 66;

    cout << "device open"<< endl;
    //デバイスのオープン
    cv::VideoCapture cap(1);

    // cap.open(1);

    // オープン失敗？
    if(!cap.isOpened())
    {
        std::cout << "ERROR: cannot open cam device." << std::endl;
        return -1;
    }

    // キャプチャサイズの設定
    cap.set(CV_CAP_PROP_FRAME_WIDTH, CAP_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAP_HEIGHT);

     cv::Rect rect(140, 105, 1000, 750);

    //ループ
    while(1)
    {
        struct timespec start_val;
		clock_gettime(CLOCK_MONOTONIC, &start_val);

        cv::Mat frame;
        // 1フレームキャプチャ
        // cap >> frame; //消すとめっちゃ早い
        if (!cap.read(frame))
        {
            std::cout << "ERROR: cannot capture." << std::endl;
            break;
        }

        cv::Mat rect_image(frame, rect);
        cv::resize(rect_image, rect_image, cv::Size(), 0.64, 0.64);
        cv::imshow("rect", rect_image);

        // gray scale conversion--------------------------
        // cv::Mat gray;
        // cv::cvtColor(frame, gray, CV_BGR2GRAY);
        //
        // cv::Mat gray(frame.rows, frame.cols, CV_8UC1);
        // cv::Vec3b* ptr = frame.ptr<cv::Vec3b>(0);
        // uchar* g_ptr = gray.ptr<uchar>(0);
        //
        // for(int y = 0; y < frame.rows * frame.cols; y = y + frame.cols){
        //     for(int x = 0; x < frame.cols; ++x){
        //         cv::Vec3b bgr = ptr[x + y];
        //         //cout << "B:" << (int)bgr[0] << ", G" << (int)bgr[1] << ", R:" <<(int)bgr[2] << endl;
        //         g_ptr[x + y] = (unsigned char)(0.114 * bgr[0] + 0.587 * bgr[1] + 0.299 * bgr[2]);
        //     }
        // }

        // binary conversion-----------------------
        cv::Mat HSV;
        cvtColor(rect_image, HSV, CV_BGR2HSV);
        cv::inRange(HSV,
            cv::Scalar(H_min, S_min, V_min, 0),
            cv::Scalar(H_max,S_max,V_max, 0),
            HSV);

        // noise remove
        // cv::Mat median;
        medianBlur(HSV, HSV, 7);
        // median_one(HSV, median, 7);
        // median_one_ptr(HSV, HSV);

        //キー待ち

        //表示
        // cv::imshow(WIN_NAME, frame);
        // cv::imshow("result", median);
        cv::imshow("HSV", HSV);


        // printf("elapsed time = ");
        // if (end.tv_nsec < start_val.tv_nsec) {
        //   printf("%5ld.%09ld", end.tv_sec - start_val.tv_sec - 1,
        //          end.tv_nsec + (long int)1.0e+9 - start_val.tv_nsec);
        // } else {
        //   printf("%5ld.%09ld", end.tv_sec - start_val.tv_sec,
        //          end.tv_nsec - start_val.tv_nsec);
        // }
        // printf(" sec \n");

        struct timespec end;
		clock_gettime(CLOCK_MONOTONIC,&end);
		long sec = end.tv_sec - start_val.tv_sec;
		long nsec = end.tv_nsec - start_val.tv_nsec;
		if(nsec < 0){
			sec--;
			nsec += 1000000000L;
		}
		msec = sec*1000+nsec/1000000;

        printf("time = %f, nsec = %ld\n", msec, nsec);
        // cout << "time = " << msec << "\n";
        // "ms, d.x = " << d.x << "pixel, " << "z = " << dis << "mm" << endl;

        if(count > 4){
            totaltime_ns += nsec;
        }
        if(count > samplingNum){
            printf("time=%ld\n", totaltime_ns);
            printf("time=%f\n", double(totaltime_ns) / (double)samplingNum - 4);
            break;
        }
        count++;

        int inp_key = cv::waitKey(1);
        //入力あり？
        if (inp_key >= 0) {
            // [ESC] or 'Q' or 'q'
            if (inp_key == 27 || inp_key == 81 || inp_key == 113) break;
            else if (inp_key == 's'){
                cv::imwrite("cam.jpg", frame);
            }
        }

    } //end of while

    //ウィンドウ破棄
    cv::destroyAllWindows();

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
    // imshow("image", dist);
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
