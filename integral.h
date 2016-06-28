#ifndef _INTEGRAL_H_
#define _INTEGRAL_H_

#include "cv.h"

/*
*功能：Convert image to single channel 32F
*参数：
*img图像
*返回值：32-bit float，[0,1]范围的图像
*/
IplImage *imtofloat32(const IplImage *img);

/*
*功能：计算积分图(wxh)
*参数：
*img，输入图像(wxh)
*返回值：积分图像(wxh),32-bit floating point,单通道
*例：积分图像：I(x,y),原图：f(x,y),I(2,1)=f(0,0)+f(1,0)+f(2,0)+f(0,1)+f(1,1)+f(2,1).
//x<=x0,y<=y0
*/
IplImage *Integral(IplImage *img);

/*
*功能：计算某一矩形范围内的像素累加值
*参数：
*img，积分图像（见函数Intergal描述），（row，col）左上角的点坐标，rows，行数，cols，列数
返回值：某一矩形范围内的像素累加值（[(row,col) , (row + rows,col + cols))）
//矩形表示：(row,col)左上角起点，rows占据的行数，cols占据的列数（包含起点所在的行和列）
*/
//! Computes the sum of pixels within the rectangle specified by the top-left start
//! co-ordinate and size
inline float BoxIntegral(IplImage *img, int row, int col, int rows, int cols)
{
	float *data = (float *)img->imageData;
	int step = img->widthStep / sizeof(float);

	// The subtraction by one for row/col is because row/col is inclusive.
	int r1 = std::min(row, img->height) - 1;
	int c1 = std::min(col, img->width) - 1;
	int r2 = std::min(row + rows, img->height) - 1;
	int c2 = std::min(col + cols, img->width) - 1;

	float A(0.0f), B(0.0f), C(0.0f), D(0.0f);
	if (r1 >= 0 && c1 >= 0) A = data[r1 * step + c1];
	if (r1 >= 0 && c2 >= 0) B = data[r1 * step + c2];
	if (r2 >= 0 && c1 >= 0) C = data[r2 * step + c1];
	if (r2 >= 0 && c2 >= 0) D = data[r2 * step + c2];

	return std::max(0.0f, A - B - C + D);
}

#endif