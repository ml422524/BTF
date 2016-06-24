#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <fstream>
#include <iostream>
#include "cv.h"

using std::ofstream;
using std::cout;
using std::endl;

/*
*功能：将数据写入文件
*参数：
*filename,文件名称
*p,数据指针
*imax,索引最大值,默认实参256
*返回值：无
*/
void writerfile(const char *filename, double *p, int imax = 256);
/*
*功能：将数据写入文件
*参数：
*filename,文件名称
*p,数据指针
*imax,索引最大值,默认实参256
*返回值：无
*/
void writerfile(const char *filename, int *p, int imax = 256);

/*
功能：将src指向的矩阵写入名为filename的文件
参数：filename，文件名；src，类型为float的矩阵
返回值：无
*/
void write2file(const char *filename, CvMat *src);

/*
功能：将src指向的图像数据写入名为filename的文件
参数：filename，文件名；src，数据类型为short的图像（与IPL_DEPTH_16S对应,单通道）
返回值：无
*/
void write2file(const char *filename, IplImage *src);

#endif