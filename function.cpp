#include "function.h"

/*
*功能：将数据写入文件
*参数：
*filename,文件名称
*p,数据指针
*imax,索引最大值,默认实参256
*返回值：无
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
*功能：将数据写入文件
*参数：
*filename,文件名称
*p,数据指针
*imax,索引最大值,默认实参256
*返回值：无
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
功能：将src指向的矩阵写入名为filename的文件
参数：filename，文件名；src，类型为float的矩阵
返回值：无
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
功能：将src指向的图像数据写入名为filename的文件
参数：filename，文件名；src，数据类型为short的图像（与IPL_DEPTH_16S对应，单通道）
返回值：无
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