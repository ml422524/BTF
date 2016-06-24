#include "ImageHist.h"

/*
*�����Ҷ�ͼ���ֱ��ͼ����
*����ֱ��ͼ���ݵ�ָ��
*/
CvHistogram *CreateGrayImageHist(IplImage **ppImage)
{
	int nHistSize = 256;
	float fRange[] = { 0, 255 };  //�Ҷȼ��ķ�Χ    
	float *pfRanges[] = { fRange };
	CvHistogram *pcvHistogram = cvCreateHist(1, &nHistSize, CV_HIST_ARRAY, pfRanges);
	cvCalcHist(ppImage, pcvHistogram);
	return pcvHistogram;
}

/*
*����ֱ��ͼ���ݴ���ֱ��ͼͼ��
*����ֱ��ͼͼ��ָ��
*/
IplImage* CreateHisogramImage(int nImageWidth, int nScale, int nImageHeight, CvHistogram *pcvHistogram)
{
	IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight),IPL_DEPTH_8U, 1);
	//��pHistImage��ָ��ͼ�����ɰ�ɫ
	cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height),
		CV_RGB(255, 255, 255), CV_FILLED);

	//ͳ��ֱ��ͼ�е����ֱ����  
	float fMaxHistValue = 0;
	cvGetMinMaxHistValue(pcvHistogram, NULL, &fMaxHistValue, NULL, NULL);

	//�ֱ�ÿ��ֱ�����ֵ���Ƶ�ͼ��  
	for (int i = 0; i < 256; i++)
	{
		float fHistValue = cvQueryHistValue_1D(pcvHistogram, i); //����Ϊi��ֱ�����С  
		//cout << fHistValue << endl;//cvCalcHist��δ�Ը���������201511132009
		int nRealHeight = cvRound((fHistValue / fMaxHistValue) * nImageHeight);  //Ҫ���Ƶĸ߶�  
		//cvRound()��һ��double�͵���������������
		cvRectangle(pHistImage,
			cvPoint(i * nScale, nImageHeight - 1),
			cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),
			cvScalar(0, 0, 0, 0),
			CV_FILLED
			);//�����Ա�����ķ�ʽ���201511132009
	}
	return pHistImage;
}
/*
����һ����ɫͼ��ת��Ϊ�Ҷ�ͼ��������ֱ��ͼ����ʾֱ��ͼ
pImageNameҪ����Ĳ�ɫͼ�������
*/
void myGrayImageHistogram(const char *pImageName)
{
	const char *pstrWindowsSrcTitle = "ԭͼ";
	const char *pstrWindowsGrayTitle = "�Ҷ�ͼ";
	const char *pstrWindowsHistTitle = "ֱ��ͼ";

	// ���ļ��м���ԭͼ  
	IplImage *pSrcImage = cvLoadImage(pImageName, CV_LOAD_IMAGE_UNCHANGED);
	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	// �Ҷ�ͼ  
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);

	// �Ҷ�ֱ��ͼ  
	CvHistogram *pcvHistogram = CreateGrayImageHist(&pGrayImage);

	// ����ֱ��ͼͼ��  
	int nHistImageWidth = 256;
	int nHistImageHeight = 150;  //ֱ��ͼͼ��߶�  
	int nScale = 2;
	IplImage *pHistImage = CreateHisogramImage(nHistImageWidth, nScale, nHistImageHeight, pcvHistogram);

	// ��ʾ  
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
�����ɫͼ�񣬽���ת��Ϊ�Ҷ�ͼ���ٴ�����ɫͼ���ֱ��ͼ���⻯ͼ��
pImageNameΪ����ͼ�������
*/
void myGrayImageEqualizeHist(const char *pImageName)
{
	const char *pstrWindowsSrcTitle = "ԭͼ";
	const char *pstrWindowsGrayTitle = "�Ҷ�ͼ";
	const char *pstrWindowsHistTitle = "ֱ��ͼ";
	const char *pstrWindowsGrayEqualizeTitle = "�Ҷ�ͼ-���⻯��";
	const char *pstrWindowsHistEqualizeTitle = "ֱ��ͼ-���⻯��";

	// ���ļ��м���ԭͼ  
	IplImage *pSrcImage = cvLoadImage(pImageName, CV_LOAD_IMAGE_UNCHANGED);
	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	// ԭͼ��ת��Ϊ�Ҷ�ͼ  
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);

	// ֱ��ͼͼ������  
	int nHistImageWidth = 256; //ͼ���� //����256û�����壬������CreateHisogramImage��֪
	int nHistImageHeight = 150;//ͼ��߶�
	int nScale = 2; //������һ�Ҷ�ֵռ�õ�������
					// �Ҷ�ֱ��ͼ��ֱ��ͼͼ��  
	CvHistogram *pcvHistogram = CreateGrayImageHist(&pGrayImage);
	IplImage *pHistImage = CreateHisogramImage(nHistImageWidth, nScale, nHistImageHeight, pcvHistogram);

	// ֱ��ͼ���⻯  	
	IplImage *pGrayEqualizeImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	cvEqualizeHist(pGrayImage, pGrayEqualizeImage);

	// ���⻯��ĻҶ�ֱ��ͼ��ֱ��ͼͼ��  
	CvHistogram *pcvHistogramEqualize = CreateGrayImageHist(&pGrayEqualizeImage);
	IplImage *pHistEqualizeImage = CreateHisogramImage(nHistImageWidth, nScale, nHistImageHeight, pcvHistogramEqualize);

	//��ʾ
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
	//�ͷ���Դ
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
*ͳ������Ҷ�ͼ��Ҷ�ֵ�Ķ�Ӧ����������ֱ��ͼ���ݣ�
*pGrayImage����Ҷ�ͼ���ָ��
*���������ٵ��ڴ�ռ��ָ�루���ڴ�ռ����ڴ洢ͼ�������лҶ�ֵ��Ӧ��������
*/
int *myCalculateGrayValueNum(IplImage *pGrayImage)
{

	uchar *pGrayImageData = (uchar *)pGrayImage->imageData;//Դ�Ҷ�ͼ��洢�ռ�ĳ�ʼ��ַ
														   //�����ڴ�ռ䣬�洢ͼ�������лҶ�ֵ��Ӧ������
	int *pImageGrayValueArr = new int[256]();

	for (int i = 0; i < pGrayImage->imageSize; ++i)
	{
		++pImageGrayValueArr[pGrayImageData[i]];
	}
	return pImageGrayValueArr;
}
/*
*���ܣ�����ֱ��ͼͼ��
*������
*pHistDataֱ��ͼ����
*nImageWidthֱ��ͼͼ���ȣ�Ĭ��ʵ��256����256��ֵδ���ƣ�
*nScaleÿһ�Ҷ�ֵռ����������Ĭ��ʵ��2
*nImageHeightֱ��ͼͼ��߶ȣ�Ĭ��ʵ��150
*����ֵ��ֱ��ͼͼ��ָ��
*/
IplImage *myCreateHistogramImage(int *pHistData, int nImageWidth, int nScale, int nImageHeight)
{
	//����ֱ��ͼͼ��
	IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight), IPL_DEPTH_8U, 1);
	//��pHistImage��ָ��ͼ�����ɰ�ɫ
	cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height),
		CV_RGB(255, 255, 255), CV_FILLED);

	//��������Ҷ�ͼ��Ҷ�ֵ�Ķ�Ӧ�����������ֵ
	double MaxHistValue = SearchMax(pHistData, 256);
	//�ֱ�ÿ��ֱ�����ֵ���Ƶ�ͼ��  
	for (int i = 0; i < 256; i++)
	{
		double HistValue = pHistData[i];

		int nRealHeight = cvRound((HistValue / MaxHistValue) * nImageHeight);  //Ҫ���Ƶĸ߶�   //cvRound()��һ��double�͵���������������
		cvRectangle(pHistImage,
			cvPoint(i * nScale, nImageHeight - 1),
			cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),
			cvScalar(0, 0, 0, 0),
			CV_FILLED
			);//�����Ա�����ķ�ʽ���201511132009
	}
	return pHistImage;
}
/*
*ֱ��ͼ����任
*pImageGrayValueArrָ��洢����Ҷ�ͼ��Ҷ�ֵ�Ķ�Ӧ���������ڳ��ռ�
*����ֱ֧��ͼ����任�������ָ��pHistData����ָ��ָ�򿪱ٵ��ڴ�ռ�
*/
double *myHistEqualizeTranform(int *pImageGrayValueArr)
{
	//�����ڴ棬�洢ֱ��ͼ����
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
		//cout << pHistData[i] << endl; //���ڵ���
	}

	return pHistData;
}
/*
*ֱ��ͼ����任����
*gv�Ҷ�ֵ��ҲΪֱ��ͼ����任����
*pImageGrayValueArrָ��Ҷ�ֵ��������
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
*����ָ��p��ֵ�洢�ռ�����ֵ
*imaxΪp��ֵ�洢�ռ�ĳߴ磬Ҳ����imaxѭ������(<imax)
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
*����ָ��p��ֵ�洢�ռ�����ֵ
*imaxΪp��ֵ�洢�ռ�ĳߴ磬Ҳ����imaxѭ������(<imax)
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
*ֱ��ͼ����任���õ��任���ͼ��
*pGrayImage����Ҷ�ͼ��ָ��
*pDst���ͼ��ָ��
*/
void myGrayImageHistEqualize(IplImage *pGrayImage, IplImage *pDst)
{
	uchar *pGrayImageData = (uchar *)pGrayImage->imageData;//Դ�Ҷ�ͼ��洢�ռ�ĳ�ʼ��ַ
	uchar *pDstStartAddr = (uchar *)pDst->imageData;

	//ͳ������Ҷ�ͼ��Ҷ�ֵ�Ķ�Ӧ��������֮��ע���ͷ�pImageGrayValueArr��ָ����ڴ�ռ�
	int *pImageGrayValueArr = myCalculateGrayValueNum(pGrayImage);
	//ֱ��ͼ����任���õ�ֱ��ͼ���ݣ�֮��ע���ͷ�pHistData��ָ����ڴ�ռ�
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
*����ֱ��ͼͼ��
*pGrayImage����Ҷ�ͼ��
*nImageWidthֱ��ͼͼ���ȣ�Ĭ��ʵ��256����256��ֵδ���ƣ�
*nScaleÿһ�Ҷ�ֵռ����������Ĭ��ʵ��2
*nImageHeightֱ��ͼͼ��߶ȣ�Ĭ��ʵ��150
*����ֱ��ͼͼ��ָ��
*/
IplImage *myCreateGrayImageHistogram(IplImage *pGrayImage, int nImageWidth, int nScale, int nImageHeight)
{
	//ͳ������Ҷ�ͼ��Ҷ�ֵ�Ķ�Ӧ��������֮��ע���ͷ�pImageGrayValueArr��ָ����ڴ�ռ�
	int *pImageGrayValueArr = myCalculateGrayValueNum(pGrayImage);

	//ֱ��ͼ����任���õ�ֱ��ͼ���ݣ�֮��ע���ͷ�pHistData��ָ����ڴ�ռ�
	//double *pHistData = myHistEqualizeTranform(pImageGrayValueArr);//20160607ע��

	//����ֱ��ͼͼ��
	IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight),IPL_DEPTH_8U, 1);
	//��pHistImage��ָ��ͼ�����ɰ�ɫ
	cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height),
		CV_RGB(255, 255, 255), CV_FILLED);

    //��������Ҷ�ͼ��Ҷ�ֵ�Ķ�Ӧ�����������ֵ
	double MaxHistValue = SearchMax(pImageGrayValueArr, 256);
	//�ֱ�ÿ��ֱ�����ֵ���Ƶ�ͼ��  
	for (int i = 0; i < 256; i++)
	{
		double HistValue = pImageGrayValueArr[i];

		int nRealHeight = cvRound((HistValue / MaxHistValue) * nImageHeight);  //Ҫ���Ƶĸ߶�  
		//cvRound()��һ��double�͵���������������
		cvRectangle(pHistImage,
			cvPoint(i * nScale, nImageHeight - 1),
			cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),
			cvScalar(0, 0, 0, 0),
			CV_FILLED
			);//�����Ա�����ķ�ʽ���201511132009
	}
	MyRealeseMemory(pImageGrayValueArr); //delete pImageGrayValueArr;
	//MyRealeseMemory(pHistData); //delete pHistData;//20160607ע��
	return pHistImage;
}
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
IplImage *myCreateColorImageNChannelHist(IplImage *pColorImage, int channel, int nImageWidth, int nScale, int nImageHeight)
{
	//ͨ������
	IplImage *pGrayImage = myChannelDeparture(pColorImage, channel);

	//ͳ������Ҷ�ͼ��Ҷ�ֵ�Ķ�Ӧ��������֮��ע���ͷ�pImageGrayValueArr��ָ����ڴ�ռ�
	int *pImageGrayValueArr = myCalculateGrayValueNum(pGrayImage);

	//����ֱ��ͼͼ��
	IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight),
		IPL_DEPTH_8U, 1);
	//��pHistImage��ָ��ͼ�����ɰ�ɫ
	cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height),
		CV_RGB(255, 255, 255), CV_FILLED);

	//��������Ҷ�ͼ��Ҷ�ֵ�Ķ�Ӧ�����������ֵ
	double MaxHistValue = SearchMax(pImageGrayValueArr, 256);
	//�ֱ�ÿ��ֱ�����ֵ���Ƶ�ͼ��  
	for (int i = 0; i < 256; i++)
	{
		double HistValue = pImageGrayValueArr[i];

		int nRealHeight = cvRound((HistValue / MaxHistValue) * nImageHeight);  //Ҫ���Ƶĸ߶�  
																			   //cvRound()��һ��double�͵���������������
		cvRectangle(pHistImage,
			cvPoint(i * nScale, nImageHeight - 1),
			cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),
			cvScalar(0, 0, 0, 0),
			CV_FILLED
			);//�����Ա�����ķ�ʽ���201511132009
	}

	MyRealeseMemory(pImageGrayValueArr); //delete[] pImageGrayValueArr;
	cvReleaseImage(&pGrayImage);

	return pHistImage;
}
/*
*���ܣ���ͨ�������ɫͼ��
*������
*pColorImage�����ɫͼ��
*channel����ɫͼ���ͨ����r��2��g��1��b��0��
*����ֵ��ĳһͨ����ɵĻҶ�ͼ
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
	//���Դ���
	//cvNamedWindow("n",1);
	//cvShowImage("n", pnChannelImage);
	//cvWaitKey(0);

	return pnChannelImage;
}
//===============================================================================
//===============================================================================
/*
*���ܣ�����Ҷ�ͼ��pGrayImage�Ĺ�һ��ֱ��ͼ����
*������
*pGrayImage����ĻҶ�ͼ��
����ֵ����һ����ֱ��ͼ����
*/
double *myNormalizedHistData(IplImage *pGrayImage)
{

	uchar *pGrayImageData = (uchar *)pGrayImage->imageData;//Դ�Ҷ�ͼ��洢�ռ�ĳ�ʼ��ַ
	//�����ڴ�ռ䣬�洢ͼ�������лҶ�ֵ��Ӧ������
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

	//���Դ���
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
*���ܣ�����Ҷ�ͼ��pGrayImage�Ĺ�һ���ۻ�ֱ��ͼ����
*������
*pGrayImage����ĻҶ�ͼ��
����ֵ����һ�����ۻ�ֱ��ͼ����
*/
double *myNormalizedCumHistData(IplImage *pGrayImage)
{

	uchar *pGrayImageData = (uchar *)pGrayImage->imageData;//Դ�Ҷ�ͼ��洢�ռ�ĳ�ʼ��ַ
														   //�����ڴ�ռ䣬�洢ͼ�������лҶ�ֵ��Ӧ������
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

	//���Դ���
	//std::cout << "pNormalizedCumHistData[255] = " << pNormalizedCumHistData[255] << std::endl;

	delete pImageGrayValueArr;

	return pNormalizedCumHistData;
}
/*
*���ܣ�����Ҷ�ͼ��pGrayImage�Ĺ�һ���ۻ�ֱ��ͼ����
*������
*pGrayImage����ĻҶ�ͼ��
*mask,���������ֱ��ͼ�ĻҶ�ֵ(��Χ��0-255)
����ֵ����һ�����ۻ�ֱ��ͼ����
*/
double *myNormalizedCumHistData(IplImage *pGrayImage, uchar mask)
{
	uchar *pGrayImageData = (uchar *)pGrayImage->imageData;//Դ�Ҷ�ͼ��洢�ռ�ĳ�ʼ��ַ
														   //�����ڴ�ռ䣬�洢ͼ�������лҶ�ֵ��Ӧ������
	int *pImageGrayValueArr = new int[256]();
	int count = 0;//��������ֱ��ͼ��������ظ���
	for (int i = 0; i < pGrayImage->imageSize; ++i)
	{
		if (pGrayImageData[i] != mask)//ֵ������mask�ĻҶ�ֵ��Ӧ�������������
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

	//���Դ���
	//std::cout << "pNormalizedCumHistData[255] = " << pNormalizedCumHistData[255] << ", count = " << count << std::endl;
	
	delete pImageGrayValueArr;

	return pNormalizedCumHistData;
}
/*
*���ܣ�����ֱ��ͼͼ��
*������
*pHistDataֱ��ͼ����
*nImageWidthֱ��ͼͼ���ȣ�Ĭ��ʵ��256����256��ֵδ���ƣ�
*nScaleÿһ�Ҷ�ֵռ����������Ĭ��ʵ��2
*nImageHeightֱ��ͼͼ��߶ȣ�Ĭ��ʵ��150
*����ֵ��ֱ��ͼͼ��ָ��
*/
IplImage *myCreateHistogramImage(double *pHistData, int nImageWidth, int nScale, int nImageHeight)
{
	//����ֱ��ͼͼ��
	IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight),
		IPL_DEPTH_8U, 1);
	//��pHistImage��ָ��ͼ�����ɰ�ɫ
	cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height),
		CV_RGB(255, 255, 255), CV_FILLED);

	//��������Ҷ�ͼ��Ҷ�ֵ�Ķ�Ӧ�����������ֵ
	double MaxHistValue = SearchMax(pHistData, 256);
	//�ֱ�ÿ��ֱ�����ֵ���Ƶ�ͼ��  
	for (int i = 0; i < 256; i++)
	{
		double HistValue = pHistData[i];

		int nRealHeight = cvRound((HistValue / MaxHistValue) * nImageHeight);  //Ҫ���Ƶĸ߶�  
																			   //cvRound()��һ��double�͵���������������
		cvRectangle(pHistImage,
			cvPoint(i * nScale, nImageHeight - 1),
			cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),
			cvScalar(0, 0, 0, 0),
			CV_FILLED
			);//�����Ա�����ķ�ʽ���201511132009
	}
	return pHistImage;
}
/*
*���ܣ�������һ��ֱ��ͼͼ��
*������
*pGrayImage�Ҷ�ͼ��
*nImageWidthֱ��ͼͼ���ȣ�Ĭ��ʵ��256����256��ֵδ���ƣ�
*nScaleÿһ�Ҷ�ֵռ����������Ĭ��ʵ��2
*nImageHeightֱ��ͼͼ��߶ȣ�Ĭ��ʵ��150
*����ֵ��ֱ��ͼͼ��ָ��
*/
IplImage *myCreateNormalizedHistogramImage(IplImage *pGrayImage, int nImageWidth , int nScale , int nImageHeight)
{
	double *pNorData = myNormalizedHistData(pGrayImage);
	IplImage *pHistImage = myCreateHistogramImage(pNorData, nImageWidth, nScale, nImageHeight);
	return pHistImage;
}

/*
*���ܣ�������һ���ۻ�ֱ��ͼͼ��
*������
*pGrayImage�Ҷ�ͼ��
*nImageWidthֱ��ͼͼ���ȣ�Ĭ��ʵ��256����256��ֵδ���ƣ�
*nScaleÿһ�Ҷ�ֵռ����������Ĭ��ʵ��2
*nImageHeightֱ��ͼͼ��߶ȣ�Ĭ��ʵ��150
*����ֵ��ֱ��ͼͼ��ָ��
*/
IplImage *myCreateNormalizedCumHistogramImage(IplImage *pGrayImage, int nImageWidth, int nScale, int nImageHeight)
{
	double *pNorCumData = myNormalizedCumHistData(pGrayImage);
	IplImage *pHistImage = myCreateHistogramImage(pNorCumData, nImageWidth, nScale, nImageHeight);
	return pHistImage;
}
//====================================================================
//====================================================================
//����Ҷ�ͼ���ֱ��ͼ����ʾ
void myCreateGrayImageHist_demo()
{
	IplImage *psrc = cvLoadImage("1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *pHist = myCreateGrayImageHistogram(psrc);

	cvNamedWindow("demo0",1);
	cvShowImage("demo0",pHist);
	cvWaitKey();
}
//�����ɫͼ��ĳһͨ����ֱ��ͼ��ʾ
void myCreateColorImageHist_demo()
{
	IplImage *psrc = cvLoadImage("1.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage *pHist = myCreateColorImageNChannelHist(psrc, 0);
	cvNamedWindow("demo1", 1);
	cvShowImage("demo1", pHist);
	cvWaitKey();

}
//����Ҷ�ͼ��Ĺ�һ��ֱ��ͼ����ʾ
void myCreateGrayImageNormalizedHist_demo()
{
	IplImage *psrc = cvLoadImage("1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *pHist = myCreateNormalizedHistogramImage(psrc);

	cvNamedWindow("demo2", 1);
	cvShowImage("demo2", pHist);
	cvWaitKey();
}
//����Ҷ�ͼ��Ĺ�һ���ۻ�ֱ��ͼ����ʾ
void myCreateGrayImageNormalizedCumHist_demo()
{
	IplImage *psrc = cvLoadImage("1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *pHist = myCreateNormalizedCumHistogramImage(psrc);

	cvNamedWindow("demo3", 1);
	cvShowImage("demo3", pHist);
	cvWaitKey();
}
//ĳЩ������֤����
void other_test()
{
	//IplImage *psrc = cvLoadImage("./cam1/a11.bmp",CV_LOAD_IMAGE_GRAYSCALE);
	//myNormalizedCumHistData(psrc);
}

