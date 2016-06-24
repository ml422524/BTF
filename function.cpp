#include "function.h"

/*
*���ܣ�������д���ļ�
*������
*filename,�ļ�����
*p,����ָ��
*imax,�������ֵ,Ĭ��ʵ��256
*����ֵ����
*/
void writerfile(const char *filename, double *p, int imax)
{
	ofstream out(filename);
	for (int i = 0; i < imax; ++i)
	{
		out << i << " " << p[i] << endl;
	}
	out.close();
}

/*
*���ܣ�������д���ļ�
*������
*filename,�ļ�����
*p,����ָ��
*imax,�������ֵ,Ĭ��ʵ��256
*����ֵ����
*/
void writerfile(const char *filename, int *p, int imax)
{
	ofstream out(filename);
	for (int i = 0; i < imax; ++i)
	{
		out << i << " " << p[i] << endl;
	}
	out.close();
}

/*
���ܣ���srcָ��ľ���д����Ϊfilename���ļ�
������filename���ļ�����src������Ϊfloat�ľ���
����ֵ����
*/
void write2file(const char *filename, CvMat *src)
{
	std::ofstream out(filename);
	if (!out)
	{
		printf("Open file failed!\n");
		return;
	}
	float data;
	for (int row = 0; row < src->rows; row++)
	{
		for (int col = 0; col < src->cols; col++)
		{
			//get data
			data = ((float*)(src->data.ptr + (size_t)src->step*row))[col];
			out << data << " ";
		}
		out << std::endl;
	}
	out.close();
	return;
}
/*
���ܣ���srcָ���ͼ������д����Ϊfilename���ļ�
������filename���ļ�����src����������Ϊshort��ͼ����IPL_DEPTH_16S��Ӧ����ͨ����
����ֵ����
*/
void write2file(const char *filename, IplImage *src)
{
	if (src->nChannels != 1)
	{
		printf("Image must be one channel!\n");
		return;
	}
	std::ofstream out(filename);
	if (!out)
	{
		printf("Open file failed!\n");
		return;
	}
	short data;
	for (int y = 0; y < src->height; ++y)
	{
		short *pdata = (short*)(src->imageData + y * src->widthStep);
		for (int x = 0; x < src->width; ++x)
		{
			//get data
			data = *(pdata + x);
			out << data << " ";
		}
		out << std::endl;
	}
	out.close();
	return;

}