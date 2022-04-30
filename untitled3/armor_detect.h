//
// Created by redamancy on 2022/4/5.
//
#pragma once
#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#include<opencv2/opencv_modules.hpp>
#include<opencv2/highgui/highgui_c.h>
#include<opencv2/imgproc/imgproc.hpp>
#define DEBUG_ARMOR_DETECT

using namespace std;
using namespace cv;

#ifdef DEBUG_ARMOR_DETECT
#define SHOW_ARMOR_PUT_TEXT
#define SHOW_ROI_RECTANGLE
#define SHOW_DRAW_SPOT
#define SHOW_DRAW_RECT
#endif

#define FAST_DISTANCE
/**
 * 灯条相关数据  角度,质心，大小（长，宽），灯条匹配状态
 */
class LED_Stick{
public:

    LED_Stick():matched(false){}

    LED_Stick(const RotatedRect & R)
    {
        rect.angle = R.angle;
        rect.center = R.center;
        rect.size = R.size;
        matched = false;

    }
    RotatedRect rect;  //装甲板灯条相关数据信息
    bool matched;  //灯条匹配状态
    size_t match_index;  //匹配相应灯条序号，用于灯条匹配
    float match_factor;  //匹配强度，用于灯条匹配
};

/**
 *装甲板相关数据信息
 */
 class armor
 {
 public:
     armor();
     armor(const LED_Stick& L1,const LED_Stick& L2);

     void draw_rect(Mat& img,Point2f roi_offset_point) const;   //画装甲板
     void draw_spot(Mat& img,Point2f roi_offset_point) const;

     int get_average_intensity(const Mat& img); //计算装甲板roi平均色彩强度，用于筛选装甲板中心有灯条
     void max_match(vector<LED_Stick>& LED,size_t i,size_t j);//匹配灯条
     bool is_suitable_size(void) const;  //判断可能的装甲板是否符合尺寸

     LED_Stick Led_stick[2];//装甲板的灯条
     float error_angle; //灯条的误差的角度
     int average_intensity;//装甲板roi的平均色彩强度
     Rect2i rect;//装甲板roi矩形
     Point2i center;//装甲板的中心点

 };

 class Armordetector
 {
 public:
     Armordetector(){};
     ~Armordetector(){};

     /**
     * @brief ArmorDetectTask 自瞄任务函数（识别，角度解算）
     * @param img
     * @param other_param 其他参数，其中使用到color颜色和cap_mode摄像头类型
     * @return 返回命令 0没发现 1发现
     */
     void DrawTarget(Mat &img)
     {
         if(!points_2d_.empty())
         {
             line(img, points_2d_[0],points_2d_[2],Scalar(255,100,0), 3);
             line(img, points_2d_[1],points_2d_[3],Scalar(255,100,0), 3);
         }
     }
     void getAngle(float &yaw, float &pitch)
     {
         yaw = angle_x_;
         pitch = angle_y_;
     }

     /**
      * 获取roi区域
      */
      Rect GetRoi(const Mat &img);
      bool makeRectSafe(cv::Rect &rect,cv::Size &size)
      {
          if(rect.x<0)
              rect.x=0;
          if(rect.x+rect.width>size.width)
              rect.width=size.width-rect.x;
          if(rect.y<0)
              rect.y=0;
          if(rect.y+rect.height>size.height)
              rect.height=size.height-rect.y;
          if(rect.width<=0 || rect.height<=0)
              return false;
          return true;
      }

      /**
       * 装甲板识别
       */
       bool DetectArmor(Mat &img,Rect rect_roi);

       /**
        *装甲板匹配
        */
       bool getTypeResult(bool is_small);
       void setFilter(int filter_size){
         filter_size_ = filter_size;
      }
       void clear(){
         history_.clear();
      }

      /**
       * 滑动条

       void callBack(int,void*);
       */

 private:
       // 外部参数
       //0为红色，1为蓝色
       int color_ = 1;
       int cap_mode_;

/** public:
     //滑动条
     int value;
*/

 public:
     // 调试参数
     int short_offset_x_ = 100;
     int short_offset_y_ = 100;
     int long_offset_x_ = 100;
     int long_offset_y_ = 100;
     int color_th_ = 30;
     int gray_th_ = 90;

 private:
     // ｒｏｉ参数
     Rect last_target_;
     int lost_cnt_ = 0;
     int detect_cnt_ = 0;

 private:
     float dist_ = 3000; // 通过距离更换相机
     float r_ = 0.5; // 距离刷新率 (0-1)
     int update_cap_cnt = 0; // 用于强制限制相机更新频率
     float distance_ = 0;
     float angle_x_ = 0;
     float angle_y_ = 0;
     vector<Point2f> points_2d_;

 private:
     // 判断大小装甲板类型相关参数
     std::list<bool> history_;
     int filter_size_ = 5;
     bool is_small_;
 };
