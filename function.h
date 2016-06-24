#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <fstream>
#include <iostream>
#include "cv.h"

using std::ofstream;
using std::cout;
using std::endl;

/*
*���ܣ�������д���ļ�
*������
*filename,�ļ�����
*p,����ָ��
*imax,�������ֵ,Ĭ��ʵ��256
*����ֵ����
*/
void writerfile(const char *filename, double *p, int imax = 256);
/*
*���ܣ�������д���ļ�
*������
*filename,�ļ�����
*p,����ָ��
*imax,�������ֵ,Ĭ��ʵ��256
*����ֵ����
*/
void writerfile(const char *filename, int *p, int imax = 256);

/*
���ܣ���srcָ��ľ���д����Ϊfilename���ļ�
������filename���ļ�����src������Ϊfloat�ľ���
����ֵ����
*/
void write2file(const char *filename, CvMat *src);

/*
���ܣ���srcָ���ͼ������д����Ϊfilename���ļ�
������filename���ļ�����src����������Ϊshort��ͼ����IPL_DEPTH_16S��Ӧ,��ͨ����
����ֵ����
*/
void write2file(const char *filename, IplImage *src);

#endif