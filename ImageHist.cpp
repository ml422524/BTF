#include "ImageHist.h"

/*
*创建灰度图像的直方图数据
*返回直方图数据的指针
*/
CvHistogram *CreateGrayImageHist(IplImage **ppImage)
{
	int nHistSize = 256;
	float fRange[] = { 0, 255 };  //灰度级的范围    
	float *pfRanges[] = { fRange };
	CvHistogram *pcvHistogram = cvCreateHist(1, &nHistSize, CV_HIST_ARRAY, pfRanges);
	cvCalcHist(ppImage, pcvHistogram);
	return pcvHistogram;
}

/*
*根据直方图数据创建直方图图像
*返回直方图图像指针
*/
IplImage* CreateHisogramImage(int nImageWidth, int nScale, int nImageHeight, CvHistogram *pcvHistogram)
{
	IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight),IPL_DEPTH_8U, 1);
	//将pHistImage所指的图像填充成白色
	cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height),
		CV_RGB(255, 255, 255), CV_FILLED);

	//统计直方图中的最大直方块  
	float fMaxHistValue = 0;
	cvGetMinMaxHistValue(pcvHistogram, NULL, &fMaxHistValue, NULL, NULL);

	//分别将每个直方块的值绘制到图中  
	for (int i = 0; i < 256; i++)
	{
		float fHistValue = cvQueryHistValue_1D(pcvHistogram, i); //像素为i的直方块大小  
		//cout << fHistValue << endl;//cvCalcHist并未以概率来计算201511132009
		int nRealHeight = cvRound((fHistValue / fMaxHistValue) * nImageHeight);  //要绘制的高度  
		//cvRound()对一个double型的数进行四舍五入
		cvRectangle(pHistImage,
			cvPoint(i * nScale, nImageHeight - 1),
			cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),
			cvScalar(0, 0, 0, 0),
			CV_FILLED
			);//两点以闭区间的方式填充201511132009
	}
	return pHistImage;
}
/*
读入一幅彩色图像，转化为灰度图，计算其直方图并显示直方图
pImageName要读入的彩色图像的名字
*/
void myGrayImageHistogram(const char *pImageName)
{
	const char *pstrWindowsSrcTitle = "原图";
	const char *pstrWindowsGrayTitle = "灰度图";
	const char *pstrWindowsHistTitle = "直方图";

	// 从文件中加载原图  
	IplImage *pSrcImage = cvLoadImage(pImageName, CV_LOAD_IMAGE_UNCHANGED);
	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	// 灰度图  
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);

	// 灰度直方图  
	CvHistogram *pcvHistogram = CreateGrayImageHist(&pGrayImage);

	// 创建直方图图像  
	int nHistImageWidth = 256;
	int nHistImageHeight = 150;  //直方图图像高度  
	int nScale = 2;
	IplImage *pHistImage = CreateHisogramImage(nHistImageWidth, nScale, nHistImageHeight, pcvHistogram);

	// 显示  
	cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(pstrWindowsGrayTitle, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(pstrWindowsHistTitle, CV_WINDOW_AUTOSIZE);
	cvShowImage(pstrWindowsSrcTitle, pSrcImage);
	cvShowImage(pstrWindowsGrayTitle, pGrayImage);
	cvShowImage(pstrWindowsHistTitle, pHistImage);

	cvWaitKey(0);

	cvReleaseHist(&pcvHistogram);

	cvDestroyWindow(pstrWindowsSrcTitle);
	cvDestroyWindow(pstrWindowsGrayTitle);
	cvDestroyWindow(pstrWindowsHistTitle);
	cvReleaseImage(&pSrcImage);
	cvReleaseImage(&pGrayImage);
	cvReleaseImage(&pHistImage);

}
/*
读入彩色图像，将其转换为灰度图，再创建灰色图像的直方图均衡化图像
pImageName为输入图像的名字
*/
void myGrayImageEqualizeHist(const char *pImageName)
{
	const char *pstrWindowsSrcTitle = "原图";
	const char *pstrWindowsGrayTitle = "灰度图";
	const char *pstrWindowsHistTitle = "直方图";
	const char *pstrWindowsGrayEqualizeTitle = "灰度图-均衡化后";
	const char *pstrWindowsHistEqualizeTitle = "直方图-均衡化后";

	// 从文件中加载原图  
	IplImage *pSrcImage = cvLoadImage(pImageName, CV_LOAD_IMAGE_UNCHANGED);
	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	// 原图像转换为灰度图  
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);

	// 直方图图像数据  
	int nHistImageWidth = 256; //图像宽度 //大于256没有意义，看函数CreateHisogramImage可知
	int nHistImageHeight = 150;//图像高度
	int nScale = 2; //横坐标一灰度值占用的像素数
					// 灰度直方图及直方图图像  
	CvHistogram *pcvHistogram = CreateGrayImageHist(&pGrayImage);
	IplImage *pHistImage = CreateHisogramImage(nHistImageWidth, nScale, nHistImageHeight, pcvHistogram);

	// 直方图均衡化  	
	IplImage *pGrayEqualizeImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	cvEqualizeHist(pGrayImage, pGrayEqualizeImage);

	// 均衡化后的灰度直方图及直方图图像  
	CvHistogram *pcvHistogramEqualize = CreateGrayImageHist(&pGrayEqualizeImage);
	IplImage *pHistEqualizeImage = CreateHisogramImage(nHistImageWidth, nScale, nHistImageHeight, pcvHistogramEqualize);

	//显示
	cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(pstrWindowsGrayTitle, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(pstrWindowsHistTitle, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(pstrWindowsGrayEqualizeTitle, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(pstrWindowsHistEqualizeTitle, CV_WINDOW_AUTOSIZE);
	cvShowImage(pstrWindowsSrcTitle, pSrcImage);
	cvShowImage(pstrWindowsGrayTitle, pGrayImage);
	cvShowImage(pstrWindowsHistTitle, pHistImage);
	cvShowImage(pstrWindowsGrayEqualizeTitle, pGrayEqualizeImage);
	cvShowImage(pstrWindowsHistEqualizeTitle, pHistEqualizeImage);

	cvWaitKey(0);
	//释放资源
	cvReleaseHist(&pcvHistogram);

	cvDestroyWindow(pstrWindowsSrcTitle);
	cvDestroyWindow(pstrWindowsGrayTitle);
	cvDestroyWindow(pstrWindowsHistTitle);
	cvDestroyWindow(pstrWindowsGrayEqualizeTitle);
	cvDestroyWindow(pstrWindowsHistEqualizeTitle);
	cvReleaseImage(&pSrcImage);
	cvReleaseImage(&pGrayImage);
	cvReleaseImage(&pHistImage);
	cvReleaseImage(&pGrayEqualizeImage);
	cvReleaseImage(&pHistEqualizeImage);

}
//====================================================================
//====================================================================

/*
*统计输入灰度图像灰度值的对应的数量（即直方图数据）
*pGrayImage输入灰度图像的指针
*返回所开辟的内存空间的指针（该内存空间用于存储图像中所有灰度值对应的数量）
*/
int *myCalculateGrayValueNum(IplImage *pGrayImage)
{

	uchar *pGrayImageData = (uchar *)pGrayImage->imageData;//源灰度图像存储空间的初始地址
														   //开辟内存空间，存储图像中所有灰度值对应的数量
	int *pImageGrayValueArr = new int[256]();

	for (int i = 0; i < pGrayImage->imageSize; ++i)
	{
		++pImageGrayValueArr[pGrayImageData[i]];
	}
	return pImageGrayValueArr;
}
/*
*功能：创建直方图图像
*参数：
*pHistData直方图数据
*nImageWidth直方图图像宽度，默认实参256（非256的值未完善）
*nScale每一灰度值占的像素数，默认实参2
*nImageHeight直方图图像高度，默认实参150
*返回值：直方图图像指针
*/
IplImage *myCreateHistogramImage(int *pHistData, int nImageWidth, int nScale, int nImageHeight)
{
	//创建直方图图像
	IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight), IPL_DEPTH_8U, 1);
	//将pHistImage所指的图像填充成白色
	cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height),
		CV_RGB(255, 255, 255), CV_FILLED);

	//搜索输入灰度图像灰度值的对应的数量的最大值
	double MaxHistValue = SearchMax(pHistData, 256);
	//分别将每个直方块的值绘制到图中  
	for (int i = 0; i < 256; i++)
	{
		double HistValue = pHistData[i];

		int nRealHeight = cvRound((HistValue / MaxHistValue) * nImageHeight);  //要绘制的高度   //cvRound()对一个double型的数进行四舍五入
		cvRectangle(pHistImage,
			cvPoint(i * nScale, nImageHeight - 1),
			cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),
			cvScalar(0, 0, 0, 0),
			CV_FILLED
			);//两点以闭区间的方式填充201511132009
	}
	return pHistImage;
}
/*
*直方图均衡变换
*pImageGrayValueArr指向存储输入灰度图像灰度值的对应的数量的内出空间
*返回支直方图均衡变换后的数组指针pHistData，该指针指向开辟的内存空间
*/
double *myHistEqualizeTranform(int *pImageGrayValueArr)
{
	//开辟内存，存储直方图数据
	double *pHistData = new double[256]();
	for (int k = 0; k < 256; ++k)
	{
		pHistData[k] = myHistTranform(k, pImageGrayValueArr);
	}
	double Tsmax = SearchMax(pHistData, 256);
	//cout << Tsmax  << endl;
	//255
	for (int i = 0; i < 256; ++i)
	{
		pHistData[i] = (uchar)(255.0 * pHistData[i] / Tsmax + 0.5);
		//cout << pHistData[i] << endl; //用于调试
	}

	return pHistData;
}
/*
*直方图均衡变换函数
*gv灰度值，也为直方图均衡变换索引
*pImageGrayValueArr指向灰度值数量数组
*/
double myHistTranform(uchar gv, int *pImageGrayValueArr)
{
	double tmp = 0;
	for (int j = 0; j <= gv; ++j)
	{
		tmp += pImageGrayValueArr[j];
	}
	return tmp;
}
/*
*搜索指针p所值存储空间的最大值
*imax为p所值存储空间的尺寸，也即，imax循环上限(<imax)
*/
double SearchMax(double *p, int imax)
{
	double max = p[0];
	for (int i = 1; i < imax; ++i)
	{
		if (p[i] > max)
			max = p[i];
	}
	return max;
}
/*
*搜索指针p所值存储空间的最大值
*imax为p所值存储空间的尺寸，也即，imax循环上限(<imax)
*/
double SearchMax(int *p, int imax)
{
	int max = p[0];
	for (int i = 1; i < imax; ++i)
	{
		if (p[i] > max)
			max = p[i];
	}
	return max;
}
/*
*直方图均衡变换，得到变换后的图像
*pGrayImage输入灰度图像指针
*pDst输出图像指针
*/
void myGrayImageHistEqualize(IplImage *pGrayImage, IplImage *pDst)
{
	uchar *pGrayImageData = (uchar *)pGrayImage->imageData;//源灰度图像存储空间的初始地址
	uchar *pDstStartAddr = (uchar *)pDst->imageData;

	//统计输入灰度图像灰度值的对应的数量，之后注意释放pImageGrayValueArr所指向的内存空间
	int *pImageGrayValueArr = myCalculateGrayValueNum(pGrayImage);
	//直方图均衡变换，得到直方图数据，之后注意释放pHistData所指向的内存空间
	double *pHistData = myHistEqualizeTranform(pImageGrayValueArr);

	for (int i = 0; i < pGrayImage->imageSize; ++i)
	{
		*(pDstStartAddr + i) = pHistData[*(pGrayImageData + i)];
	}
	MyRealeseMemory(pImageGrayValueArr); //delete[] pImageGrayValueArr;
	MyRealeseMemory(pHistData); //delete[] pHistData;
}
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
IplImage *myCreateGrayImageHistogram(IplImage *pGrayImage, int nImageWidth, int nScale, int nImageHeight)
{
	//统计输入灰度图像灰度值的对应的数量，之后注意释放pImageGrayValueArr所指向的内存空间
	int *pImageGrayValueArr = myCalculateGrayValueNum(pGrayImage);

	//直方图均衡变换，得到直方图数据，之后注意释放pHistData所指向的内存空间
	//double *pHistData = myHistEqualizeTranform(pImageGrayValueArr);//20160607注释

	//创建直方图图像
	IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight),IPL_DEPTH_8U, 1);
	//将pHistImage所指的图像填充成白色
	cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height),
		CV_RGB(255, 255, 255), CV_FILLED);

    //搜索输入灰度图像灰度值的对应的数量的最大值
	double MaxHistValue = SearchMax(pImageGrayValueArr, 256);
	//分别将每个直方块的值绘制到图中  
	for (int i = 0; i < 256; i++)
	{
		double HistValue = pImageGrayValueArr[i];

		int nRealHeight = cvRound((HistValue / MaxHistValue) * nImageHeight);  //要绘制的高度  
		//cvRound()对一个double型的数进行四舍五入
		cvRectangle(pHistImage,
			cvPoint(i * nScale, nImageHeight - 1),
			cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),
			cvScalar(0, 0, 0, 0),
			CV_FILLED
			);//两点以闭区间的方式填充201511132009
	}
	MyRealeseMemory(pImageGrayValueArr); //delete pImageGrayValueArr;
	//MyRealeseMemory(pHistData); //delete pHistData;//20160607注释
	return pHistImage;
}
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
IplImage *myCreateColorImageNChannelHist(IplImage *pColorImage, int channel, int nImageWidth, int nScale, int nImageHeight)
{
	//通道分离
	IplImage *pGrayImage = myChannelDeparture(pColorImage, channel);

	//统计输入灰度图像灰度值的对应的数量，之后注意释放pImageGrayValueArr所指向的内存空间
	int *pImageGrayValueArr = myCalculateGrayValueNum(pGrayImage);

	//创建直方图图像
	IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight),
		IPL_DEPTH_8U, 1);
	//将pHistImage所指的图像填充成白色
	cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height),
		CV_RGB(255, 255, 255), CV_FILLED);

	//搜索输入灰度图像灰度值的对应的数量的最大值
	double MaxHistValue = SearchMax(pImageGrayValueArr, 256);
	//分别将每个直方块的值绘制到图中  
	for (int i = 0; i < 256; i++)
	{
		double HistValue = pImageGrayValueArr[i];

		int nRealHeight = cvRound((HistValue / MaxHistValue) * nImageHeight);  //要绘制的高度  
																			   //cvRound()对一个double型的数进行四舍五入
		cvRectangle(pHistImage,
			cvPoint(i * nScale, nImageHeight - 1),
			cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),
			cvScalar(0, 0, 0, 0),
			CV_FILLED
			);//两点以闭区间的方式填充201511132009
	}

	MyRealeseMemory(pImageGrayValueArr); //delete[] pImageGrayValueArr;
	cvReleaseImage(&pGrayImage);

	return pHistImage;
}
/*
*功能：按通道分离彩色图像
*参数：
*pColorImage输入彩色图像
*channel，彩色图像的通道（r，2；g，1；b，0）
*返回值：某一通道组成的灰度图
*/
IplImage *myChannelDeparture(IplImage *pColorImage, int channel)
{
	if (pColorImage->nChannels != 3)
	{
		std::cout << "pColorImage->nChannels is not three !" << std::endl; exit(0);
	}
	IplImage *pnChannelImage = cvCreateImage(cvGetSize(pColorImage), IPL_DEPTH_8U, 1);
	uchar *pndata = (uchar *)pnChannelImage->imageData;
	uchar *pColorImagedata = (uchar *)pColorImage->imageData;
	for (int y = 0; y < pnChannelImage->height; ++y)
	{
		for (int x = 0; x < pnChannelImage->width; ++x)
		{
			*(pndata + y * pnChannelImage->widthStep + x) = *(pColorImagedata + y * pColorImage->widthStep + x * pColorImage->nChannels + channel);
		}
	}
	//测试代码
	//cvNamedWindow("n",1);
	//cvShowImage("n", pnChannelImage);
	//cvWaitKey(0);

	return pnChannelImage;
}
//===============================================================================
//===============================================================================
/*
*功能：计算灰度图像pGrayImage的归一化直方图数据
*参数：
*pGrayImage输入的灰度图像
返回值：归一化的直方图数据
*/
double *myNormalizedHistData(IplImage *pGrayImage)
{

	uchar *pGrayImageData = (uchar *)pGrayImage->imageData;//源灰度图像存储空间的初始地址
	//开辟内存空间，存储图像中所有灰度值对应的数量
	int *pImageGrayValueArr = new int[256]();
	for (int i = 0; i < pGrayImage->imageSize; ++i)
	{
		++pImageGrayValueArr[pGrayImageData[i]];
	}
	double *pNormalizedHistData = new double[256];
	for (int i = 0; i < 256; ++i)
	{
		pNormalizedHistData[i] = static_cast<double>(pImageGrayValueArr[i]) / static_cast<double>(pGrayImage->imageSize);//imageSize = height * widthStep
	}

	//测试代码
	//double sum = 0;
	//for (int i = 0; i < 256; ++i)
	//{
	//	sum += pNormalizedHistData[i];
	//}

	//std::cout << "sum = " << sum << std::endl;

	delete pImageGrayValueArr;

	return pNormalizedHistData;
}
/*
*功能：计算灰度图像pGrayImage的归一化累积直方图数据
*参数：
*pGrayImage输入的灰度图像
返回值：归一化的累积直方图数据
*/
double *myNormalizedCumHistData(IplImage *pGrayImage)
{

	uchar *pGrayImageData = (uchar *)pGrayImage->imageData;//源灰度图像存储空间的初始地址
														   //开辟内存空间，存储图像中所有灰度值对应的数量
	int *pImageGrayValueArr = new int[256]();
	for (int i = 0; i < pGrayImage->imageSize; ++i)
	{
		++pImageGrayValueArr[pGrayImageData[i]];
	}
	double *pNormalizedCumHistData = new double[256];
	double sum = 0;
	for (int i = 0; i < 256; ++i)
	{
		pNormalizedCumHistData[i] = static_cast<double>(pImageGrayValueArr[i]) / static_cast<double>(pGrayImage->imageSize);//imageSize = height * widthStep
		sum += pNormalizedCumHistData[i];
		pNormalizedCumHistData[i] = sum;
	}

	//测试代码
	//std::cout << "pNormalizedCumHistData[255] = " << pNormalizedCumHistData[255] << std::endl;

	delete pImageGrayValueArr;

	return pNormalizedCumHistData;
}
/*
*功能：计算灰度图像pGrayImage的归一化累积直方图数据
*参数：
*pGrayImage输入的灰度图像
*mask,不纳入计算直方图的灰度值(范围：0-255)
返回值：归一化的累积直方图数据
*/
double *myNormalizedCumHistData(IplImage *pGrayImage, uchar mask)
{
	uchar *pGrayImageData = (uchar *)pGrayImage->imageData;//源灰度图像存储空间的初始地址
														   //开辟内存空间，存储图像中所有灰度值对应的数量
	int *pImageGrayValueArr = new int[256]();
	int count = 0;//计算纳入直方图计算的像素个数
	for (int i = 0; i < pGrayImage->imageSize; ++i)
	{
		if (pGrayImageData[i] != mask)//值不等于mask的灰度值对应的像素纳入计算
		{
			++pImageGrayValueArr[pGrayImageData[i]];
			++count;
		}
	}
	double *pNormalizedCumHistData = new double[256];
	double sum = 0;
	for (int i = 0; i < 256; ++i)
	{
		pNormalizedCumHistData[i] = static_cast<double>(pImageGrayValueArr[i]) / static_cast<double>(count);
		sum += pNormalizedCumHistData[i];
		pNormalizedCumHistData[i] = sum;
	}

	//测试代码
	//std::cout << "pNormalizedCumHistData[255] = " << pNormalizedCumHistData[255] << ", count = " << count << std::endl;
	
	delete pImageGrayValueArr;

	return pNormalizedCumHistData;
}
/*
*功能：创建直方图图像
*参数：
*pHistData直方图数据
*nImageWidth直方图图像宽度，默认实参256（非256的值未完善）
*nScale每一灰度值占的像素数，默认实参2
*nImageHeight直方图图像高度，默认实参150
*返回值：直方图图像指针
*/
IplImage *myCreateHistogramImage(double *pHistData, int nImageWidth, int nScale, int nImageHeight)
{
	//创建直方图图像
	IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight),
		IPL_DEPTH_8U, 1);
	//将pHistImage所指的图像填充成白色
	cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height),
		CV_RGB(255, 255, 255), CV_FILLED);

	//搜索输入灰度图像灰度值的对应的数量的最大值
	double MaxHistValue = SearchMax(pHistData, 256);
	//分别将每个直方块的值绘制到图中  
	for (int i = 0; i < 256; i++)
	{
		double HistValue = pHistData[i];

		int nRealHeight = cvRound((HistValue / MaxHistValue) * nImageHeight);  //要绘制的高度  
																			   //cvRound()对一个double型的数进行四舍五入
		cvRectangle(pHistImage,
			cvPoint(i * nScale, nImageHeight - 1),
			cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),
			cvScalar(0, 0, 0, 0),
			CV_FILLED
			);//两点以闭区间的方式填充201511132009
	}
	return pHistImage;
}
/*
*功能：创建归一化直方图图像
*参数：
*pGrayImage灰度图像
*nImageWidth直方图图像宽度，默认实参256（非256的值未完善）
*nScale每一灰度值占的像素数，默认实参2
*nImageHeight直方图图像高度，默认实参150
*返回值：直方图图像指针
*/
IplImage *myCreateNormalizedHistogramImage(IplImage *pGrayImage, int nImageWidth , int nScale , int nImageHeight)
{
	double *pNorData = myNormalizedHistData(pGrayImage);
	IplImage *pHistImage = myCreateHistogramImage(pNorData, nImageWidth, nScale, nImageHeight);
	return pHistImage;
}

/*
*功能：创建归一化累积直方图图像
*参数：
*pGrayImage灰度图像
*nImageWidth直方图图像宽度，默认实参256（非256的值未完善）
*nScale每一灰度值占的像素数，默认实参2
*nImageHeight直方图图像高度，默认实参150
*返回值：直方图图像指针
*/
IplImage *myCreateNormalizedCumHistogramImage(IplImage *pGrayImage, int nImageWidth, int nScale, int nImageHeight)
{
	double *pNorCumData = myNormalizedCumHistData(pGrayImage);
	IplImage *pHistImage = myCreateHistogramImage(pNorCumData, nImageWidth, nScale, nImageHeight);
	return pHistImage;
}
//====================================================================
//====================================================================
//计算灰度图像的直方图的演示
void myCreateGrayImageHist_demo()
{
	IplImage *psrc = cvLoadImage("1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *pHist = myCreateGrayImageHistogram(psrc);

	cvNamedWindow("demo0",1);
	cvShowImage("demo0",pHist);
	cvWaitKey();
}
//计算彩色图像某一通道的直方图演示
void myCreateColorImageHist_demo()
{
	IplImage *psrc = cvLoadImage("1.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage *pHist = myCreateColorImageNChannelHist(psrc, 0);
	cvNamedWindow("demo1", 1);
	cvShowImage("demo1", pHist);
	cvWaitKey();

}
//计算灰度图像的归一化直方图的演示
void myCreateGrayImageNormalizedHist_demo()
{
	IplImage *psrc = cvLoadImage("1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *pHist = myCreateNormalizedHistogramImage(psrc);

	cvNamedWindow("demo2", 1);
	cvShowImage("demo2", pHist);
	cvWaitKey();
}
//计算灰度图像的归一化累积直方图的演示
void myCreateGrayImageNormalizedCumHist_demo()
{
	IplImage *psrc = cvLoadImage("1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *pHist = myCreateNormalizedCumHistogramImage(psrc);

	cvNamedWindow("demo3", 1);
	cvShowImage("demo3", pHist);
	cvWaitKey();
}
//某些功能验证测试
void other_test()
{
	//IplImage *psrc = cvLoadImage("./cam1/a11.bmp",CV_LOAD_IMAGE_GRAYSCALE);
	//myNormalizedCumHistData(psrc);
}

