#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "armor_detect.h"

using namespace cv;
using namespace std;

//static void on_ContrastAndBright(int, void*);

int main() {
    Mat video1(200,200,CV_32FC2,Scalar(0));
    Mat new_image(200,200,CV_32FC2,Scalar(0));
    int g_nContrastValue = 100; //对比度值
    int g_nBrightValue = -150; //亮度值
    namedWindow("效果图",1);
    createTrackbar("对比度调节","效果图",&g_nContrastValue,300);
    createTrackbar("亮度调节","效果图",&g_nBrightValue,200);
    VideoCapture video("/home/redamancy/下载/ood_blue.mp4");
    namedWindow("识别窗口", CV_WINDOW_AUTOSIZE);
    /**while (1) {
        video.read(video1);
        new_image = video1.clone();//深拷贝
        for (int y = 0; y < video1.rows; y++) {
            for (int x = 0; x < video1.cols; x++) {
                for (c = 0; c < 3; c++) //c为通道数
                {
                    if (video1.c() == 3) {
                        float b = video1.at<Vec3b>(y, x)[0];
                        float g = video1.at<Vec3b>(y, x)[1];
                        float r = video1.at<Vec3b>(y, x)[2];

                        dst.at<Vec3b>(y, x)[0] = saturate_cast<uchar>(b*g_nContrastValue/10 + g_nBrightValue);
                        dst.at<Vec3b>(y, x)[1] = saturate_cast<uchar>(g*g_nContrastValue/10+ g_nBrightValue);
                        dst.at<Vec3b>(y, x)[2] = saturate_cast<uchar>(r*g_nContrastValue/10 + g_nBrightValue);

                    }
                    else if (video1.c() == 1) {
                        float v_gray = video1.at<uchar>(y, x);
                        dst.at<uchar>(y, x) = saturate_cast<uchar>(v_gray + g_nBrightValue);
                    }
                    //new_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(
                            //(g_nContrastValue * 0.01) * (video1.at<Vec3b>(y, x)[c]) + g_nBrightValue);
                }
            }
        }*/
    while(1)
    {
        video.read(video1);
        new_image=video1.clone();
        for(int y = 0; y < video1.rows; y++ )
        {
            for(int x = 0; x < video1.cols; x++ )
            {
                for(int c = 0; c < 3; c++ )//c为通道数
                {
                    new_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(
                            (g_nContrastValue * 0.01) * (video1.at<Vec3b>(y, x)[c]) + g_nBrightValue);
                    //Vec3b可以看作是vector<uchar, 3>，即一个uchar类型、长度为3的vector向量。
                }
            }


        }
        imshow("窗口图像", new_image);
        Armordetector armordetector;
        armor arm;
        Rect roi = armordetector.GetRoi(new_image);
        armordetector.DetectArmor(new_image, roi);
        imshow("detect", new_image);
        if (waitKey(1) == 27) break;

    }

        return 0;
}

