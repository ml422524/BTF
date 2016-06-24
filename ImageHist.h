#ifndef _GRAYIMAGEHIST
#define _GRAYIMAGEHIST

#include "cv.h"
#include "highgui.h"

/*
*创建灰度图像的直方图数据
*返回直方图数据的指针
*/
CvHistogram* CreateGrayImageHist(IplImage **ppImage);
/*
*根据直方图数据创建直方图图像 
*返回直方图图像指针
*/
IplImage* CreateHisogramImage(int nImageWidth, int nScale, int nImageHeight, CvHistogram *pcvHistogram);
/*
读入一幅彩色图像，转化为灰度图，计算其直方图并显示直方图
pImageName要读入的彩色图像的名字
*/
void myGrayImageHistogram(const char *pImageName);
/*
读入彩色图像，将其转换为灰度图，再创建灰色图像的直方图均衡化图像
pImageName为输入图像的名字
*/
void myGrayImageEqualizeHist(const char *pImageName);
//====================================================================
//====================================================================

/*
*统计输入灰度图像灰度值的对应的数量（即直方图数据）
*pGrayImage输入灰度图像的指针
*返回所开辟的内存空间的指针（该内存空间用于存储图像中所有灰度值对应的数量）
*/
int *myCalculateGrayValueNum(IplImage *pGrayImage);
/*
*功能：创建直方图图像
*参数：
*pHistData直方图数据
*nImageWidth直方图图像宽度，默认实参256（非256的值未完善）
*nScale每一灰度值占的像素数，默认实参2
*nImageHeight直方图图像高度，默认实参150
*返回值：直方图图像指针
*/
IplImage *myCreateHistogramImage(int *pHistData, int nImageWidth = 256, int nScale = 2, int nImageHeight = 150);
/*
*直方图均衡变换
*pImageGrayValueArr指向存储输入灰度图像灰度值的对应的数量的内存空间
*返回支直方图均衡变换后的数组指针pHistData，该指针指向开辟的内存空间
*/
double *myHistEqualizeTranform(int *pImageGrayValueArr);
/*
*直方图均衡变换函数
*gv灰度值，也为直方图均衡变换索引
*pImageGrayValueArr指向灰度值数量数组
*/
double myHistTranform(uchar gv, int *pImageGrayValueArr);

/*
*搜索指针p所值存储空间的最大值
*imax为p所值存储空间的尺寸，也即，imax循环上限(<imax)
*/
double SearchMax(double *p, int imax);
double SearchMax(int *p, int imax);
/*
*释放内存
*/
inline void MyRealeseMemory(int *p)
{
	delete[] p;
}
/*
*释放内存
*/
inline void MyRealeseMemory(double *p)
{
	delete[] p;
}
/*
*直方图均衡变换，得到变换后的图像
*pGrayImage输入灰度图像指针
*pDst输出图像指针
*/
void myGrayImageHistEqualize(IplImage *pGrayImage, IplImage *pDst);
//===============================================================================
//===============================================================================
/*
*创建直方图图像
*pGrayImage输入灰度图像
*nImageWidth直方图图像宽度，默认实参256（非256的值未完善）
*nScale每一灰度值占的像素数，默认实参2
*nImageHeight直方图图像高度，默认实参150
*返回直方图图像指针
*/
IplImage *myCreateGrayImageHistogram(IplImage *pGrayImage, int nImageWidth = 256, int nScale = 2, int nImageHeight = 150);
/*
*功能：创建直方图图像
*参数：
*pColorImage输入彩色图像
*channel，彩色图像的通道（r，2；g，1；b，0）
*nImageWidth直方图图像宽度，默认实参256（非256的值未完善）
*nScale每一灰度值占的像素数，默认实参2
*nImageHeight直方图图像高度，默认实参150
*返回值：返回直方图图像指针
*/
IplImage *myCreateColorImageNChannelHist(IplImage *pColorImage, int channel,int nImageWidth = 256, int nScale = 2, int nImageHeight = 150);
/*
*功能：分离彩色图像
*参数：
*pColorImage输入彩色图像
*channel，彩色图像的通道（r，2；g，1；b，0）
*返回值：某一通道组成的灰度图
*/
IplImage *myChannelDeparture(IplImage *pColorImage, int channel);
//===============================================================================
//===============================================================================
/*
*功能：计算灰度图像pGrayImage的归一化直方图数据
*参数：
*pGrayImage输入的灰度图像
返回值：归一化的直方图数据
*/
double *myNormalizedHistData(IplImage *pGrayImage);
/*
*功能：计算灰度图像pGrayImage的归一化累积直方图数据
*参数：
*pGrayImage输入的灰度图像
返回值：归一化的累积直方图数据
*/
double *myNormalizedCumHistData(IplImage *pGrayImage);

/*
*功能：计算灰度图像pGrayImage的归一化累积直方图数据
*参数：
*pGrayImage输入的灰度图像
*mask,不纳入计算直方图的灰度值(范围：0-255)
返回值：归一化的累积直方图数据
*/
double *myNormalizedCumHistData(IplImage *pGrayImage,uchar mask);

/*
*功能：创建直方图图像
*参数：
*pHistData直方图数据
*nImageWidth直方图图像宽度，默认实参256（非256的值未完善）
*nScale每一灰度值占的像素数，默认实参2
*nImageHeight直方图图像高度，默认实参150
*返回值：直方图图像指针
*/
IplImage *myCreateHistogramImage(double *pHistData, int nImageWidth = 256, int nScale = 2, int nImageHeight = 150);
/*
*功能：创建归一化直方图图像
*参数：
*pGrayImage灰度图像
*nImageWidth直方图图像宽度，默认实参256（非256的值未完善）
*nScale每一灰度值占的像素数，默认实参2
*nImageHeight直方图图像高度，默认实参150
*返回值：直方图图像指针
*/
IplImage *myCreateNormalizedHistogramImage(IplImage *pGrayImage, int nImageWidth = 256, int nScale = 2, int nImageHeight = 150);
/*
*功能：创建归一化累积直方图图像
*参数：
*pGrayImage灰度图像
*nImageWidth直方图图像宽度，默认实参256（非256的值未完善）
*nScale每一灰度值占的像素数，默认实参2
*nImageHeight直方图图像高度，默认实参150
*返回值：直方图图像指针
*/
IplImage *myCreateNormalizedCumHistogramImage(IplImage *pGrayImage, int nImageWidth = 256, int nScale = 2, int nImageHeight = 150);

//====================================================================
//====================================================================
//计算灰度图像的直方图的演示
void myCreateGrayImageHist_demo();
//计算彩色图像某一通道的直方图演示
void myCreateColorImageHist_demo();
//计算灰度图像的归一化直方图的演示
void myCreateGrayImageNormalizedHist_demo();
//计算灰度图像的归一化累积直方图的演示
void myCreateGrayImageNormalizedCumHist_demo();
//某些功能验证测试
void other_test();

#endif