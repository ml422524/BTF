#ifndef _OBJECTDETECTION_H_
#define _OBJECTDETECTION_H_

#include "highgui.h"
#include "cv.h"
#include "regions.h"
#include "function.h"
#include "variable.h"
#include <vector>
using std::vector;

//背景减除法简单测试
void bkdminus_demo();

/*
*功能：检测运动目标（使用条件：场景不变，目标在动；使用方法：背景减除法）
*参数：
*psrc为原图指针，pbkd为背景图像指针（psrc，pbkd不限通道数，如果非灰度图像，则建立灰度图像副本；如果是灰度图像，则不转换）
*返回值:坐标
*/
vector<CvRect> objdetect_bkrndminus(IplImage *psrc,IplImage *pbkd);

/*
*功能：计算两幅图像之差（pimg1-pimg2）
*参数：
*pimg1，pimg2灰度图像，pres保存相减的结果(函数中暂时只支持IPL_DEPTH_8S，IPL_DEPTH_16S，IPL_DEPTH_32S的深度)
*返回值：相减之后的最大与最小值，存于类MinMax
*/
MinMax mcvSub(IplImage *pimg1,IplImage *pimg2,IplImage *pres);

/*
*功能：图像相减，并取绝对值
*参数：
*pimg1，pimg2灰度图像，pres保存相减的结果(函数中暂时只支持IPL_DEPTH_8S，IPL_DEPTH_16S，IPL_DEPTH_32S的深度)
*返回值：相减之后的最大与最小值，存于类MinMax
*/
MinMax mcvSubAbs(IplImage *pimg1, IplImage *pimg2, IplImage *pres);

/*
*功能：二值化(函数中src暂时只支持IPL_DEPTH_8S，IPL_DEPTH_16S，IPL_DEPTH_32S的深度)
*/
void mcvThreshold(IplImage *src,IplImage *dst,double threshold,double max_value,int type);

/*
*功能：根据图像最大最小值，压缩至0-255范围(src暂时只支持IPL_DEPTH_16S)
*参数：
*src，输入图像；mm，最小最大值对象
*返回值：灰度图像
*/
IplImage *mcvImageLinearCompress(IplImage *src,const MinMax &mm);

/*
*功能：在图像上画矩形框
*参数：
*img,图像；vRect，矩形坐标向量；color，颜色
*返回值：无
*/
void mcvDrawRect(IplImage *img,const vector<CvRect> &vRect,CvScalar color);

#endif