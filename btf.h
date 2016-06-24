#ifndef _BTF_H_
#define _BTF_H_

#include <memory>
#include "ImageHist.h"
#include "function.h"
/*
*���ܣ���������ת��������BTF��������/�ɹ�һ���ۻ�ֱ��ͼ����/����Ŀ�귶Χ�����Ա���
*������
*img1,img2����δȥ�������Ĳ�ɫͼ���ں�����ȥ������������ʱʹ���ֶ�ȥ��������ͼƬ��
*channel,ͨ������r��2��g��1��b��0��
*direction:ӳ�䷽��;0:img1->img2(img1��img2������),1:img2->img1(img2��img1������)
*����ֵ������ת������������
*/
int *myBTF(IplImage *img1, IplImage *img2, int channel, int direction);

/*
*���ܣ���pָ����ڴ�ռ��У�������num��ӽ���ֵ�������ظ�ֵ��Ӧ������
*������
*num�������ο�ֵ
*p��ָ���ڴ�ռ�(�ڱ����̾��������£��̶�Ϊ256)
*����ֵ����num��ӽ���ֵ������
*/
int SearchBestMatch(double num, double *p);

//BTF����ʾ
void btf_demo();

#endif
