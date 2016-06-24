#ifndef _BTF_H_
#define _BTF_H_

#include <memory>
#include "ImageHist.h"
#include "function.h"
/*
*功能：计算亮度转化函数（BTF）的数据/由归一化累积直方图计算/考虑目标范围，忽略背景
*参数：
*img1,img2两幅未去除背景的彩色图像，在函数中去除背景（现暂时使用手动去除背景的图片）
*channel,通道；（r，2；g，1，b，0）
*direction:映射方向;0:img1->img2(img1有img2的亮度),1:img2->img1(img2有img1的亮度)
*返回值：亮度转换函数的数据
*/
int *myBTF(IplImage *img1, IplImage *img2, int channel, int direction);

/*
*功能：在p指向的内存空间中，搜索与num最接近的值，并返回该值对应的索引
*参数：
*num，搜索参考值
*p，指向内存空间(在本工程具体问题下，固定为256)
*返回值：与num最接近的值的索引
*/
int SearchBestMatch(double num, double *p);

//BTF简单演示
void btf_demo();

#endif
