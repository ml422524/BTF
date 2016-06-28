#ifndef _INTEGRAL_H_
#define _INTEGRAL_H_

#include "cv.h"

/*
*���ܣ�Convert image to single channel 32F
*������
*imgͼ��
*����ֵ��32-bit float��[0,1]��Χ��ͼ��
*/
IplImage *imtofloat32(const IplImage *img);

/*
*���ܣ��������ͼ(wxh)
*������
*img������ͼ��(wxh)
*����ֵ������ͼ��(wxh),32-bit floating point,��ͨ��
*��������ͼ��I(x,y),ԭͼ��f(x,y),I(2,1)=f(0,0)+f(1,0)+f(2,0)+f(0,1)+f(1,1)+f(2,1).
//x<=x0,y<=y0
*/
IplImage *Integral(IplImage *img);

/*
*���ܣ�����ĳһ���η�Χ�ڵ������ۼ�ֵ
*������
*img������ͼ�񣨼�����Intergal����������row��col�����Ͻǵĵ����꣬rows��������cols������
����ֵ��ĳһ���η�Χ�ڵ������ۼ�ֵ��[(row,col) , (row + rows,col + cols))��
//���α�ʾ��(row,col)���Ͻ���㣬rowsռ�ݵ�������colsռ�ݵ�����������������ڵ��к��У�
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