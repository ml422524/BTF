#ifndef _GRAYIMAGEHIST
#define _GRAYIMAGEHIST

#include "cv.h"
#include "highgui.h"

/*
*�����Ҷ�ͼ���ֱ��ͼ����
*����ֱ��ͼ���ݵ�ָ��
*/
CvHistogram* CreateGrayImageHist(IplImage **ppImage);
/*
*����ֱ��ͼ���ݴ���ֱ��ͼͼ�� 
*����ֱ��ͼͼ��ָ��
*/
IplImage* CreateHisogramImage(int nImageWidth, int nScale, int nImageHeight, CvHistogram *pcvHistogram);
/*
����һ����ɫͼ��ת��Ϊ�Ҷ�ͼ��������ֱ��ͼ����ʾֱ��ͼ
pImageNameҪ����Ĳ�ɫͼ�������
*/
void myGrayImageHistogram(const char *pImageName);
/*
�����ɫͼ�񣬽���ת��Ϊ�Ҷ�ͼ���ٴ�����ɫͼ���ֱ��ͼ���⻯ͼ��
pImageNameΪ����ͼ�������
*/
void myGrayImageEqualizeHist(const char *pImageName);
//====================================================================
//====================================================================

/*
*ͳ������Ҷ�ͼ��Ҷ�ֵ�Ķ�Ӧ����������ֱ��ͼ���ݣ�
*pGrayImage����Ҷ�ͼ���ָ��
*���������ٵ��ڴ�ռ��ָ�루���ڴ�ռ����ڴ洢ͼ�������лҶ�ֵ��Ӧ��������
*/
int *myCalculateGrayValueNum(IplImage *pGrayImage);
/*
*���ܣ�����ֱ��ͼͼ��
*������
*pHistDataֱ��ͼ����
*nImageWidthֱ��ͼͼ���ȣ�Ĭ��ʵ��256����256��ֵδ���ƣ�
*nScaleÿһ�Ҷ�ֵռ����������Ĭ��ʵ��2
*nImageHeightֱ��ͼͼ��߶ȣ�Ĭ��ʵ��150
*����ֵ��ֱ��ͼͼ��ָ��
*/
IplImage *myCreateHistogramImage(int *pHistData, int nImageWidth = 256, int nScale = 2, int nImageHeight = 150);
/*
*ֱ��ͼ����任
*pImageGrayValueArrָ��洢����Ҷ�ͼ��Ҷ�ֵ�Ķ�Ӧ���������ڴ�ռ�
*����ֱ֧��ͼ����任�������ָ��pHistData����ָ��ָ�򿪱ٵ��ڴ�ռ�
*/
double *myHistEqualizeTranform(int *pImageGrayValueArr);
/*
*ֱ��ͼ����任����
*gv�Ҷ�ֵ��ҲΪֱ��ͼ����任����
*pImageGrayValueArrָ��Ҷ�ֵ��������
*/
double myHistTranform(uchar gv, int *pImageGrayValueArr);

/*
*����ָ��p��ֵ�洢�ռ�����ֵ
*imaxΪp��ֵ�洢�ռ�ĳߴ磬Ҳ����imaxѭ������(<imax)
*/
double SearchMax(double *p, int imax);
double SearchMax(int *p, int imax);
/*
*�ͷ��ڴ�
*/
inline void MyRealeseMemory(int *p)
{
	delete[] p;
}
/*
*�ͷ��ڴ�
*/
inline void MyRealeseMemory(double *p)
{
	delete[] p;
}
/*
*ֱ��ͼ����任���õ��任���ͼ��
*pGrayImage����Ҷ�ͼ��ָ��
*pDst���ͼ��ָ��
*/
void myGrayImageHistEqualize(IplImage *pGrayImage, IplImage *pDst);
//===============================================================================
//===============================================================================
/*
*����ֱ��ͼͼ��
*pGrayImage����Ҷ�ͼ��
*nImageWidthֱ��ͼͼ���ȣ�Ĭ��ʵ��256����256��ֵδ���ƣ�
*nScaleÿһ�Ҷ�ֵռ����������Ĭ��ʵ��2
*nImageHeightֱ��ͼͼ��߶ȣ�Ĭ��ʵ��150
*����ֱ��ͼͼ��ָ��
*/
IplImage *myCreateGrayImageHistogram(IplImage *pGrayImage, int nImageWidth = 256, int nScale = 2, int nImageHeight = 150);
/*
*���ܣ�����ֱ��ͼͼ��
*������
*pColorImage�����ɫͼ��
*channel����ɫͼ���ͨ����r��2��g��1��b��0��
*nImageWidthֱ��ͼͼ���ȣ�Ĭ��ʵ��256����256��ֵδ���ƣ�
*nScaleÿһ�Ҷ�ֵռ����������Ĭ��ʵ��2
*nImageHeightֱ��ͼͼ��߶ȣ�Ĭ��ʵ��150
*����ֵ������ֱ��ͼͼ��ָ��
*/
IplImage *myCreateColorImageNChannelHist(IplImage *pColorImage, int channel,int nImageWidth = 256, int nScale = 2, int nImageHeight = 150);
/*
*���ܣ������ɫͼ��
*������
*pColorImage�����ɫͼ��
*channel����ɫͼ���ͨ����r��2��g��1��b��0��
*����ֵ��ĳһͨ����ɵĻҶ�ͼ
*/
IplImage *myChannelDeparture(IplImage *pColorImage, int channel);
//===============================================================================
//===============================================================================
/*
*���ܣ�����Ҷ�ͼ��pGrayImage�Ĺ�һ��ֱ��ͼ����
*������
*pGrayImage����ĻҶ�ͼ��
����ֵ����һ����ֱ��ͼ����
*/
double *myNormalizedHistData(IplImage *pGrayImage);
/*
*���ܣ�����Ҷ�ͼ��pGrayImage�Ĺ�һ���ۻ�ֱ��ͼ����
*������
*pGrayImage����ĻҶ�ͼ��
����ֵ����һ�����ۻ�ֱ��ͼ����
*/
double *myNormalizedCumHistData(IplImage *pGrayImage);

/*
*���ܣ�����Ҷ�ͼ��pGrayImage�Ĺ�һ���ۻ�ֱ��ͼ����
*������
*pGrayImage����ĻҶ�ͼ��
*mask,���������ֱ��ͼ�ĻҶ�ֵ(��Χ��0-255)
����ֵ����һ�����ۻ�ֱ��ͼ����
*/
double *myNormalizedCumHistData(IplImage *pGrayImage,uchar mask);

/*
*���ܣ�����ֱ��ͼͼ��
*������
*pHistDataֱ��ͼ����
*nImageWidthֱ��ͼͼ���ȣ�Ĭ��ʵ��256����256��ֵδ���ƣ�
*nScaleÿһ�Ҷ�ֵռ����������Ĭ��ʵ��2
*nImageHeightֱ��ͼͼ��߶ȣ�Ĭ��ʵ��150
*����ֵ��ֱ��ͼͼ��ָ��
*/
IplImage *myCreateHistogramImage(double *pHistData, int nImageWidth = 256, int nScale = 2, int nImageHeight = 150);
/*
*���ܣ�������һ��ֱ��ͼͼ��
*������
*pGrayImage�Ҷ�ͼ��
*nImageWidthֱ��ͼͼ���ȣ�Ĭ��ʵ��256����256��ֵδ���ƣ�
*nScaleÿһ�Ҷ�ֵռ����������Ĭ��ʵ��2
*nImageHeightֱ��ͼͼ��߶ȣ�Ĭ��ʵ��150
*����ֵ��ֱ��ͼͼ��ָ��
*/
IplImage *myCreateNormalizedHistogramImage(IplImage *pGrayImage, int nImageWidth = 256, int nScale = 2, int nImageHeight = 150);
/*
*���ܣ�������һ���ۻ�ֱ��ͼͼ��
*������
*pGrayImage�Ҷ�ͼ��
*nImageWidthֱ��ͼͼ���ȣ�Ĭ��ʵ��256����256��ֵδ���ƣ�
*nScaleÿһ�Ҷ�ֵռ����������Ĭ��ʵ��2
*nImageHeightֱ��ͼͼ��߶ȣ�Ĭ��ʵ��150
*����ֵ��ֱ��ͼͼ��ָ��
*/
IplImage *myCreateNormalizedCumHistogramImage(IplImage *pGrayImage, int nImageWidth = 256, int nScale = 2, int nImageHeight = 150);

//====================================================================
//====================================================================
//����Ҷ�ͼ���ֱ��ͼ����ʾ
void myCreateGrayImageHist_demo();
//�����ɫͼ��ĳһͨ����ֱ��ͼ��ʾ
void myCreateColorImageHist_demo();
//����Ҷ�ͼ��Ĺ�һ��ֱ��ͼ����ʾ
void myCreateGrayImageNormalizedHist_demo();
//����Ҷ�ͼ��Ĺ�һ���ۻ�ֱ��ͼ����ʾ
void myCreateGrayImageNormalizedCumHist_demo();
//ĳЩ������֤����
void other_test();

#endif