#include "btf.h"


/*
*���ܣ���������ת��������BTF��������/�ɹ�һ���ۻ�ֱ��ͼ����/����Ŀ�귶Χ�����Ա���
*������
*img1,img2����δȥ�������Ĳ�ɫͼ���ں�����ȥ������������ʱʹ���ֶ�ȥ��������ͼƬ��
*channel,ͨ������r��2��g��1��b��0��
*direction:ӳ�䷽��;0:img1->img2(img1��img2������),1:img2->img1(img2��img1������)
*����ֵ������ת������������
*/
int *myBTF(IplImage *img1, IplImage *img2, int channel,int direction)
{
	assert(img1->nChannels == 3 && img2->nChannels == 3);

	IplImage *pImg1 = myChannelDeparture(img1, channel);

	IplImage *pImg2 = myChannelDeparture(img2,channel);

	double *p1 = myNormalizedCumHistData(pImg1,0);

	double *p2 = myNormalizedCumHistData(pImg2,0);

	writerfile("./txtfiles/file1.txt", p1);
	writerfile("./txtfiles/file2.txt", p2);

	//����洢�ռ�
	int *pres = new int[256];
	//��Ӵ��������ۻ�ֱ��ͼ�Ĵ��룬�õ�BTF
	if (direction == 0)//img1->img2(img1��img2������)
	{
		for (int i = 0; i < 256; ++i)
		{
			pres[i] = SearchBestMatch(p1[i],p2);

		}

	}
	else if (direction == 1)//img2->img1(img2��img1������)
	{
		for (int i = 0; i < 256; ++i)
		{
			pres[i] = SearchBestMatch(p2[i], p1);
		}
	}//end if

	//clean up
	delete[] p1;
	delete[] p2;
	cvReleaseImage(&pImg1);
	cvReleaseImage(&pImg2);

	return pres;
}


/*
*���ܣ���pָ����ڴ�ռ��У�������num��ӽ���ֵ�������ظ�ֵ��Ӧ������
*������
*num�������ο�ֵ
*p��ָ���ڴ�ռ�(�ڱ����̾��������£��̶�Ϊ256)
*����ֵ����num��ӽ���ֵ������
*/
int SearchBestMatch(double num,double *p)
{
	int idx = -1;
	for (int i = 0; i < 255; ++i)
	{
		//�ҵ�С����
		if ((p[i] <= num) && (p[i + 1] >= num))
		{
			//�ҵ���ӽ���
			if ((num - p[i]) <= (p[i + 1] - num))
				idx = i;
			else
				idx = i + 1;
			//����forѭ��
			break;
		}
	}
	return idx;
}
//BTF����ʾ
void btf_demo()
{
	IplImage *pImg1 = cvLoadImage("./pic/cam1/a11.bmp");
	IplImage *pImg2 = cvLoadImage("./pic/cam2/a11.bmp");
	int *p = myBTF(pImg1, pImg2, 0, 0);
	writerfile("./txt/files/map.txt", p);
	delete[] p;
}
