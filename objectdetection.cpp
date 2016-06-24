#include "objectdetection.h"

//背景减除法简单测试
void bkdminus_demo()
{
	const char *pbkdTitle = "src";
	const char *pImgTitle = "img";
	IplImage *pbkd = cvLoadImage(string(respicrpath + "/bkd.jpg").c_str());
	IplImage *pimg = cvLoadImage(string(respicrpath + "/frame.jpg").c_str());

	vector<CvRect> vRect = objdetect_bkrndminus(pimg, pbkd);

	cvNamedWindow(pbkdTitle);
	cvShowImage(pbkdTitle, pbkd);

	//画目标框
	mcvDrawRect(pimg,vRect,cvScalar(255.0));

	cvNamedWindow(pImgTitle);
	cvShowImage(pImgTitle, pimg);

	cvWaitKey(0);

	cvDestroyWindow(pbkdTitle);
	cvReleaseImage(&pbkd);
	cvDestroyWindow(pImgTitle);
	cvReleaseImage(&pimg);
}

/*
*功能：检测运动目标（使用条件：场景不变，目标在动；使用方法：背景减除法）
*参数：
*psrc为原图指针，pbkd为背景图像指针（psrc，pbkd不限通道数，如果非灰度图像，则建立灰度图像副本；如果是灰度图像，则不转换）
*返回值:坐标
*/
vector<CvRect> objdetect_bkrndminus(IplImage *psrc, IplImage *pbkd)
{
	vector<CvRect> vRect;
	//如果图像大小不一致
	if ((psrc->width != pbkd->width) || (psrc->height != pbkd->height))
	{
		return vRect;
	}
	//如果图像大小一致
	//若不是灰度图，则全部转化为灰度图	
	bool psrcflag = false;//标志量
	bool pbkdflag = false;//标志量

	if (psrc->nChannels != 1)
	{
		IplImage *psrcgray = cvCreateImage(cvSize(psrc->width,psrc->height),IPL_DEPTH_8U,1);
		cvCvtColor(psrc,psrcgray,CV_RGB2GRAY);
		psrc = psrcgray;
		psrcflag = true;
	}
	if (pbkd->nChannels != 1)
	{
		IplImage *pbkdgray = cvCreateImage(cvSize(pbkd->width, pbkd->height), IPL_DEPTH_8U, 1);
		cvCvtColor(pbkd, pbkdgray, CV_RGB2GRAY);
		pbkd = pbkdgray;
		pbkdflag = true;
	}
	//图像相减
	IplImage *pres = cvCreateImage(cvSize(psrc->width,psrc->height),IPL_DEPTH_16S,1);//保存相减的结果

	//psrc减pbkd,结果存于pres;返会差值的最大最小值
	MinMax minmax = mcvSubAbs(psrc,pbkd,pres);
	//------------------------------------------
	//建立pbw保存pres的二值化结果
	IplImage *pbw = cvCreateImage(cvSize(psrc->width, psrc->height), IPL_DEPTH_8U, 1);
	mcvThreshold(pres, pbw, (minmax.max - minmax.min) * 5.0 / 10.0 + minmax.min, 255.0, CV_THRESH_BINARY);
	cout << minmax.min << " " << minmax.max << " " << (minmax.max - minmax.min) * 5.0 / 10.0 + minmax.min << endl;
	cvNamedWindow("pbw");
	cvShowImage("pbw", pbw);
	cvWaitKey(0);
	cvDestroyWindow("pbw");
	cvReleaseImage(&pbw);
	//-----------------------------------------
	//----------------------------------------------
	//将图片压缩至0-255范围,用于直观感受相减的结果
	IplImage *dst = mcvImageLinearCompress(pres,minmax);
	cvNamedWindow("dst");
	cvShowImage("dst", dst);
	cvWaitKey(0);
	cvDestroyWindow("dst");
	cvReleaseImage(&dst);
	//----------------------------------------------
	//----------------------------------------------
	//确定目标区域
	//自定义5*5,参考点（3,3）的矩形模板
	IplConvKernel *myModel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_RECT);
	//膨胀
	cvDilate(pbw, pbw, myModel, 1);	
	//create storage for contours
	CvMemStorage *storage = cvCreateMemStorage(0);
	CvRect rect;  CvSeq *contours = 0;
	//draw bounding box around each contour
	for (; contours != 0; contours = contours->h_next)
	{
		//extract bounding box for current contour
		double area = fabs(cvContourArea(contours));
		if (area > 500)//此处可能会继续优化，确定某一范围为所需的目标，例如(500,1000)
		{
			rect = cvBoundingRect(contours, 0);
			//加入vRect
			vRect.push_back(rect);
		}
	}
	//----------------------------------------------

	write2file(string(restfwpath+"/subImage.txt").c_str(),pres);
	//释放资源
	if (psrcflag)
	{
		cvReleaseImage(&psrc);
	}
	if (pbkdflag)
	{
		cvReleaseImage(&pbkd);
	}
	cvReleaseImage(&pres);
	//返回位置向量
	return vRect;
}

/*
*功能：计算两幅图像之差（pimg1-pimg2）
*参数：
*pimg1，pimg2灰度图像，pres保存相减的结果(函数中暂时只支持IPL_DEPTH_8S，IPL_DEPTH_16S，IPL_DEPTH_32S的深度)
*返回值：相减之后的最大与最小值，存于类MinMax
*/
MinMax mcvSub(IplImage *pimg1, IplImage *pimg2, IplImage *pres)
{
	//检验是否为灰度图
	if (pimg1->nChannels != 1 || pimg2->nChannels != 1)
	{
		printf("input image must be gray images!\n"); exit(0);
	}
	int min,max;//保存最小最大值
	for (int y = 0; y < pimg1->height; ++y)
	{
		//获取每行的初始地址
		uchar *p1 = (uchar *)(pimg1->imageData + y * pimg1->widthStep);
		uchar *p2 = (uchar *)(pimg2->imageData + y * pimg2->widthStep);
		switch ((unsigned int)pres->depth)
		{
			case IPL_DEPTH_8S://IPL_DEPTH_8S单独的话当无符号处理
			{
				//printf("IPL_DEPTH_8S\n");//验证用
				char *ps = (char*)(pres->imageData + y * pres->widthStep);
				for (int x = 0; x < pimg1->width; ++x)
				{
					//计算并设置差值
					ps[x] = p1[x] - p2[x];
					//求最小最大值
					if (y == 0 && x == 0)
					{
						min = ps[x]; max = ps[x];
					}
					if (ps[x] < min)
						min = ps[x];
					if (ps[x] > max)
						max = ps[x];
				}
			}
				break;
			case IPL_DEPTH_16S://IPL_DEPTH_16S单独的话当无符号处理
			{
				//printf("IPL_DEPTH_16S\n");//验证用
				short *ps = (short*)(pres->imageData + y * pres->widthStep);
				for (int x = 0; x < pimg1->width; ++x)
				{
					//计算并设置差值
					ps[x] = p1[x] - p2[x];
					//求最小最大值
					if (y == 0 && x == 0)
					{
						min = ps[x]; max = ps[x];
					}
					if (ps[x] < min)
						min = ps[x];
					if (ps[x] > max)
						max = ps[x];
				}
			}
				break;
			case IPL_DEPTH_32S://IPL_DEPTH_32S单独的话当无符号处理
			{
				//printf("IPL_DEPTH_32S\n");//验证用
				int *ps = (int*)(pres->imageData + y * pres->widthStep);
				for (int x = 0; x < pimg1->width; ++x)
				{
					//计算并设置差值
					ps[x] = p1[x] - p2[x];
					//求最小最大值
					if (y == 0 && x == 0)
					{
						min = ps[x]; max = ps[x];
					}
					if (ps[x] < min)
						min = ps[x];
					if (ps[x] > max)
						max = ps[x];
				}
			}
				break;
			default:
				printf("pres's depth isn't supported!\n"); exit(0);
				break;
		}//end switch-case
	}//end for
	return MinMax(min,max);
}

/*
*功能：图像相减，并取绝对值
*参数：
*pimg1，pimg2灰度图像，pres保存相减的结果(函数中暂时只支持IPL_DEPTH_8S，IPL_DEPTH_16S，IPL_DEPTH_32S的深度)
*返回值：相减之后的最大与最小值，存于类MinMax
*/
MinMax mcvSubAbs(IplImage *pimg1, IplImage *pimg2, IplImage *pres)
{
	//检验是否为灰度图
	if (pimg1->nChannels != 1 || pimg2->nChannels != 1)
	{
		printf("input image must be gray images!\n"); exit(0);
	}
	int min, max;//保存最小最大值
	for (int y = 0; y < pimg1->height; ++y)
	{
		//获取每行的初始地址
		uchar *p1 = (uchar *)(pimg1->imageData + y * pimg1->widthStep);
		uchar *p2 = (uchar *)(pimg2->imageData + y * pimg2->widthStep);
		switch ((unsigned int)pres->depth)
		{
		case IPL_DEPTH_8S://IPL_DEPTH_8S单独的话当无符号处理
		{
			//printf("IPL_DEPTH_8S\n");//验证用
			char *ps = (char*)(pres->imageData + y * pres->widthStep);
			for (int x = 0; x < pimg1->width; ++x)
			{
				//计算并设置差值(取绝对值)
				if (p1[x] - p2[x] < 0)
					ps[x] = p2[x] - p1[x];
				else
					ps[x] = p1[x] - p2[x];
				//求最小最大值
				if (y == 0 && x == 0)
				{
					min = ps[x]; max = ps[x];
				}
				if (ps[x] < min)
					min = ps[x];
				if (ps[x] > max)
					max = ps[x];
			}
		}
		break;
		case IPL_DEPTH_16S://IPL_DEPTH_16S单独的话当无符号处理
		{
			//printf("IPL_DEPTH_16S\n");//验证用
			short *ps = (short*)(pres->imageData + y * pres->widthStep);
			for (int x = 0; x < pimg1->width; ++x)
			{
				//计算并设置差值(取绝对值)
				if (p1[x] - p2[x] < 0)
					ps[x] = p2[x] - p1[x];
				else
					ps[x] = p1[x] - p2[x];
				//求最小最大值
				if (y == 0 && x == 0)
				{
					min = ps[x]; max = ps[x];
				}
				if (ps[x] < min)
					min = ps[x];
				if (ps[x] > max)
					max = ps[x];
			}
		}
		break;
		case IPL_DEPTH_32S://IPL_DEPTH_32S单独的话当无符号处理
		{
			//printf("IPL_DEPTH_32S\n");//验证用
			int *ps = (int*)(pres->imageData + y * pres->widthStep);
			for (int x = 0; x < pimg1->width; ++x)
			{
				//计算并设置差值(取绝对值)
				if (p1[x] - p2[x] < 0)
					ps[x] = p2[x] - p1[x];
				else
					ps[x] = p1[x] - p2[x];
				//求最小最大值
				if (y == 0 && x == 0)
				{
					min = ps[x]; max = ps[x];
				}
				if (ps[x] < min)
					min = ps[x];
				if (ps[x] > max)
					max = ps[x];
			}
		}
		break;
		default:
			printf("pres's depth isn't supported!\n"); exit(0);
			break;
		}//end switch-case
	}//end for
	return MinMax(min, max);
}

/*
*功能：二值化(函数中src暂时只支持IPL_DEPTH_8S，IPL_DEPTH_16S，IPL_DEPTH_32S的深度)
*/
void mcvThreshold(IplImage *src, IplImage *dst, double threshold, double max_value, int type)
{
	for (int y = 0; y < src->height; ++y)
	{
		switch ((unsigned int)src->depth)
		{
			case IPL_DEPTH_8S://IPL_DEPTH_8S单独的话当无符号处理
			{
				//printf("IPL_DEPTH_8S\n");//验证用
				char *ps = (char*)(src->imageData + y * src->widthStep);
				for (int x = 0; x < src->width; ++x)
				{
					switch (type)
					{
						case CV_THRESH_BINARY:
							if (ps[x] > threshold)
								*(dst->imageData + y * dst->widthStep + x) = max_value;
							else
								*(dst->imageData + y * dst->widthStep + x) = 0;
							break;
						case CV_THRESH_BINARY_INV:
							if (ps[x] > threshold)
								*(dst->imageData + y * dst->widthStep + x) = 0;
							else
								*(dst->imageData + y * dst->widthStep + x) = max_value;
							break;
						default:
							break;
					}//end switch-case
				}//end for
			}
			break;
		case IPL_DEPTH_16S://IPL_DEPTH_16S单独的话当无符号处理
		{
			//printf("IPL_DEPTH_16S\n");//验证用
			short *ps = (short*)(src->imageData + y * src->widthStep);
			for (int x = 0; x < src->width; ++x)
			{
				switch (type)
				{
				case CV_THRESH_BINARY:
					if (ps[x] > threshold)
						*(dst->imageData + y * dst->widthStep + x) = max_value;
					else
						*(dst->imageData + y * dst->widthStep + x) = 0;
					break;
				case CV_THRESH_BINARY_INV:
					if (ps[x] > threshold)
						*(dst->imageData + y * dst->widthStep + x) = 0;
					else
						*(dst->imageData + y * dst->widthStep + x) = max_value;
					break;
				default:
					break;
				}//end switch-case
			}//end for
		}
		break;
		case IPL_DEPTH_32S://IPL_DEPTH_32S单独的话当无符号处理
		{
			//printf("IPL_DEPTH_32S\n");//验证用
			int *ps = (int*)(src->imageData + y * src->widthStep);
			for (int x = 0; x < src->width; ++x)
			{
				switch (type)
				{
				case CV_THRESH_BINARY:
					if (ps[x] > threshold)
						*(dst->imageData + y * dst->widthStep + x) = max_value;
					else
						*(dst->imageData + y * dst->widthStep + x) = 0;
					break;
				case CV_THRESH_BINARY_INV:
					if (ps[x] > threshold)
						*(dst->imageData + y * dst->widthStep + x) = 0;
					else
						*(dst->imageData + y * dst->widthStep + x) = max_value;
					break;
				default:
					break;
				}//end switch-case
			}//end for
		}
		break;
		default:
			printf("pres's depth isn't supported!\n"); exit(0);
			break;
		}//end switch-case
	}//end for

}

/*
*功能：根据图像最大最小值，压缩至0-255范围(src暂时只支持IPL_DEPTH_16S)
*参数：
*src，输入图像；mm，最小最大值对象
*返回值：灰度图像
*/
IplImage *mcvImageLinearCompress(IplImage *src, const MinMax &mm)
{
	IplImage *dst = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);

	double k = 255.0 / (mm.max - mm.min);
	int res = 0;
	short *ps = NULL;
	uchar *pdst = NULL;
	for (int y = 0; y < src->height; ++y)
	{
		ps = (short *)(src->imageData + y * src->widthStep);
		pdst = (uchar *)(dst->imageData + y * dst->widthStep);
		for (int x = 0; x < src->width; ++x)
		{
			res = static_cast<int>(k * (ps[x] - mm.min) + 0.5);
			if (res > 255)
				res = 255;
			*(pdst + x) = res;
		}
	}
	return dst;
}

/*
*功能：在图像上画矩形框
*参数：
*img,图像；vRect，矩形坐标向量；color，颜色
*返回值：无
*/
void mcvDrawRect(IplImage *img, const vector<CvRect> &vRect, CvScalar color)
{
	int size = vRect.size();
	for (int i = 0; i < size; ++i)
	{
		//画图时以两点坐标的闭区间形式画图，若超出边界，则保持原样，即不画矩形；
		cvRectangle(img,cvPoint(vRect[i].x,vRect[i].y),
			cvPoint(vRect[i].x + vRect[i].width, vRect[i].y + vRect[i].height),
			color);
	}
}
