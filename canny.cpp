#include "canny.h"

IplImage *g_pScrImage;
IplImage *g_pCannyImage;
const char *pstrWinCannyTitle = "Canny";
const char *pstrWinScrTitle = "Souce Image(May have Changed)";

void on_trackbar(int threshold)
{
	std::cout << "Call Back! Threshold is :" << threshold << std::endl;
	cvCanny(g_pScrImage, g_pCannyImage, threshold, 2*threshold, 3);
	cvShowImage(pstrWinCannyTitle, g_pCannyImage);
}
void myCanny_demo()
{
	const char *pstrImageName = "./pic/lena.jpg";
	//��ȡԭͼ
	g_pScrImage = cvLoadImage(pstrImageName, CV_LOAD_IMAGE_GRAYSCALE);
	//�����洢�����ͼ��
	g_pCannyImage = cvCreateImage(cvGetSize(g_pScrImage),IPL_DEPTH_8U,1);
	//����������ʾcanny������Ĵ���
	cvNamedWindow(pstrWinCannyTitle, 1); //�ô��ڱ������ڵ���cvCreateTrackbar֮ǰ����������trackbar������ʾ�ڴ�����

	//�����������������ûص�����
	int nThresholdEdge = 10;//�������α��ʼͣ����λ��
	cvCreateTrackbar("Threshold", pstrWinCannyTitle,&nThresholdEdge,100, on_trackbar);

	on_trackbar(nThresholdEdge);

	cvNamedWindow(pstrWinScrTitle, 1);
	cvShowImage(pstrWinScrTitle, g_pScrImage);

	cvWaitKey(0);
	cvDestroyWindow(pstrWinCannyTitle);
	cvDestroyWindow(pstrWinScrTitle);
	cvReleaseImage(&g_pCannyImage);
	cvReleaseImage(&g_pScrImage);
}