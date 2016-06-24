#ifndef _OBJECTDETECTION_H_
#define _OBJECTDETECTION_H_

#include "highgui.h"
#include "cv.h"
#include "regions.h"
#include "function.h"
#include "variable.h"

/*
*���ܣ�����˶�Ŀ�꣨ʹ���������������䣬Ŀ���ڶ���ʹ�÷�����������������
*������
*psrcΪԭͼָ�룬pbkdΪ����ͼ��ָ�루psrc��pbkd����ͨ����������ǻҶ�ͼ�������Ҷ�ͼ�񸱱�������ǻҶ�ͼ����ת����
*����ֵ:����
*/
Rect objdetect_bkrndminus(IplImage *psrc,IplImage *pbkd);

/*
*���ܣ���������ͼ��֮�pimg1-pimg2��
*������
*pimg1��pimg2�Ҷ�ͼ��pres��������Ľ��
*����ֵ�����֮����������Сֵ��������MinMax
*/
MinMax mcvSub(IplImage *pimg1,IplImage *pimg2,IplImage *pres);

/*
*���ܣ���ֵ��(������src��ʱֻ֧��IPL_DEPTH_8S��IPL_DEPTH_16S��IPL_DEPTH_32S�����)
*/
void mcvThreshold(IplImage *src,IplImage *dst,double threshold,double max_value,int type);

/*
*���ܣ�����ͼ�������Сֵ��ѹ����0-255��Χ(src��ʱֻ֧��IPL_DEPTH_16S)
*������
*src������ͼ��mm����С���ֵ����
*����ֵ���Ҷ�ͼ��
*/
IplImage *mcvImageLinearCompress(IplImage *src,const MinMax &mm);


//�����������򵥲���
void bkdminus_demo();

#endif