#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <fstream>
#include <iostream>
#include "cv.h"
#include "ipoint.h"
#include <vector>
#include "highgui.h"

using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;
using std::vector;

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

//-----------------------------------------------------
/*Below functions are used in SURF*/

//! Show the provided image and wait for keypress
void showImage(const IplImage *img);

//! Show the provided image in titled window and wait for keypress
void showImage(char *title, const IplImage *img);

//! Draw a single feature on the image
void drawIpoint(IplImage *img, Ipoint &ipt, int tailSize = 0);

//! Draw all the Ipoints in the provided vector
void drawIpoints(IplImage *img, std::vector<Ipoint> &ipts, int tailSize = 0);

//! Draw descriptor windows around Ipoints in the provided vector
void drawWindows(IplImage *img, std::vector<Ipoint> &ipts);

// Draw the FPS figure on the image (requires at least 2 calls)
void drawFPS(IplImage *img);

//! Draw a Point at feature location
void drawPoint(IplImage *img, Ipoint &ipt);

//! Draw a Point at all features
void drawPoints(IplImage *img, std::vector<Ipoint> &ipts);

//! Save the SURF features to file
void saveSurf(char *filename, std::vector<Ipoint> &ipts);

//! Load the SURF features from file
void loadSurf(char *filename, std::vector<Ipoint> &ipts);

//! Round float to nearest integer
inline int fRound(float flt)
{
	return (int)floor(flt + 0.5f);
}
/*Above functions are used in SURF*/
//----------------------------------------------------------
#endif