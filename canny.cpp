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
	//读取原图
	g_pScrImage = cvLoadImage(pstrImageName, CV_LOAD_IMAGE_GRAYSCALE);
	//创建存储结果的图像
	g_pCannyImage = cvCreateImage(cvGetSize(g_pScrImage),IPL_DEPTH_8U,1);
	//创建窗口显示canny检测结果的窗口
	cvNamedWindow(pstrWinCannyTitle, 1); //该窗口必须先于调用cvCreateTrackbar之前创建，否则trackbar不会显示在窗口上

	//创建滑动条，并设置回掉函数
	int nThresholdEdge = 10;//滑动条游标初始停留的位置
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