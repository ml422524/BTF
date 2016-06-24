#include "btf.h"


/*
*功能：计算亮度转化函数（BTF）的数据/由归一化累积直方图计算/考虑目标范围，忽略背景
*参数：
*img1,img2两幅未去除背景的彩色图像，在函数中去除背景（现暂时使用手动去除背景的图片）
*channel,通道；（r，2；g，1，b，0）
*direction:映射方向;0:img1->img2(img1有img2的亮度),1:img2->img1(img2有img1的亮度)
*返回值：亮度转换函数的数据
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

	//分配存储空间
	int *pres = new int[256];
	//添加处理两条累积直方图的代码，得到BTF
	if (direction == 0)//img1->img2(img1有img2的亮度)
	{
		for (int i = 0; i < 256; ++i)
		{
			pres[i] = SearchBestMatch(p1[i],p2);

		}

	}
	else if (direction == 1)//img2->img1(img2有img1的亮度)
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
*功能：在p指向的内存空间中，搜索与num最接近的值，并返回该值对应的索引
*参数：
*num，搜索参考值
*p，指向内存空间(在本工程具体问题下，固定为256)
*返回值：与num最接近的值的索引
*/
int SearchBestMatch(double num,double *p)
{
	int idx = -1;
	for (int i = 0; i < 255; ++i)
	{
		//找到小区间
		if ((p[i] <= num) && (p[i + 1] >= num))
		{
			//找到最接近的
			if ((num - p[i]) <= (p[i + 1] - num))
				idx = i;
			else
				idx = i + 1;
			//跳出for循环
			break;
		}
	}
	return idx;
}
//BTF简单演示
void btf_demo()
{
	IplImage *pImg1 = cvLoadImage("./pic/cam1/a11.bmp");
	IplImage *pImg2 = cvLoadImage("./pic/cam2/a11.bmp");
	int *p = myBTF(pImg1, pImg2, 0, 0);
	writerfile("./txt/files/map.txt", p);
	delete[] p;
}
