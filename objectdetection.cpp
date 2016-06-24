#include "objectdetection.h"

/*
*���ܣ�����˶�Ŀ�꣨ʹ���������������䣬Ŀ���ڶ���ʹ�÷�����������������
*������
*psrcΪԭͼָ�룬pbkdΪ����ͼ��ָ�루psrc��pbkd����ͨ����������ǻҶ�ͼ�������Ҷ�ͼ�񸱱�������ǻҶ�ͼ����ת����
*����ֵ:����
*/
Rect objdetect_bkrndminus(IplImage *psrc, IplImage *pbkd)
{
	//���ͼ���С��һ��
	if ((psrc->width != pbkd->width) || (psrc->height != pbkd->height))
	{
		return Rect(-1,-1,0,0);
	}
	//���ͼ���Сһ��
	//�����ǻҶ�ͼ����ȫ��ת��Ϊ�Ҷ�ͼ	
	bool psrcflag = false;//��־��
	bool pbkdflag = false;//��־��

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
	//ͼ�����
	IplImage *pres = cvCreateImage(cvSize(psrc->width,psrc->height),IPL_DEPTH_16S,1);//��������Ľ��

	//psrc��pbkd,�������pres;�����ֵ�������Сֵ
	MinMax minmax = mcvSub(psrc,pbkd,pres);
	//------------------------------------------
	////��ͼƬ�����Ŀ�귵�صĿ���Ϊ����Ҳ����Ϊ����ʹ�ö�ֵ�������ף���Ϊ������ʽ
	////����pbw����pres�Ķ�ֵ�����
	////IplImage *pbw = cvCreateImage(cvSize(psrc->width, psrc->height), IPL_DEPTH_8U, 1);
	////cout << minmax.min << " " << minmax.max << " " << (minmax.max - minmax.min) * 7.0 / 10.0 + minmax.min << endl;
	////mcvThreshold(pres, pbw, (minmax.max - minmax.min) * 7.0 / 10.0 + minmax.min, 255.0, CV_THRESH_BINARY);
	//////mcvThreshold(pres, pbw, (minmax.max - minmax.min) * 7.0 / 10.0 + minmax.min, 255.0, CV_THRESH_BINARY_INV);
	////cvNamedWindow("pbw");
	////cvShowImage("pbw", pbw);
	////cvWaitKey(0);
	////cvDestroyWindow("pbw");
	////cvReleaseImage(&pbw);
	//-----------------------------------------
	//----------------------------------------------
	//��ͼƬѹ����0-255��Χ,����ֱ�۸�������Ľ��
	//IplImage *dst = mcvImageLinearCompress(pres,minmax);
	//cvNamedWindow("dst");
	//cvShowImage("dst", dst);
	//cvWaitKey(0);
	//cvDestroyWindow("dst");
	//cvReleaseImage(&dst);
	//----------------------------------------------
	//----------------------------------------------
	//ȷ��Ŀ������
	//
	//----------------------------------------------

	write2file(string(restfwpath+"/subImage.txt").c_str(),pres);
	//�ͷ���Դ
	if (psrcflag)
	{
		cvReleaseImage(&psrc);
	}
	if (pbkdflag)
	{
		cvReleaseImage(&pbkd);
	}
	cvReleaseImage(&pres);
	//����λ��
	return Rect();
}

/*
*���ܣ���������ͼ��֮�pimg1-pimg2��
*������
*pimg1��pimg2�Ҷ�ͼ��pres��������Ľ��(��������ʱֻ֧��IPL_DEPTH_8S��IPL_DEPTH_16S��IPL_DEPTH_32S�����)
*����ֵ�����֮����������Сֵ��������MinMax
*/
MinMax mcvSub(IplImage *pimg1, IplImage *pimg2, IplImage *pres)
{
	//�����Ƿ�Ϊ�Ҷ�ͼ
	if (pimg1->nChannels != 1 || pimg2->nChannels != 1)
	{
		printf("input image must be gray images!\n"); exit(0);
	}
	int min,max;//������С���ֵ
	for (int y = 0; y < pimg1->height; ++y)
	{
		//��ȡÿ�еĳ�ʼ��ַ
		uchar *p1 = (uchar *)(pimg1->imageData + y * pimg1->widthStep);
		uchar *p2 = (uchar *)(pimg2->imageData + y * pimg2->widthStep);
		switch ((unsigned int)pres->depth)
		{
			case IPL_DEPTH_8S://IPL_DEPTH_8S�����Ļ����޷��Ŵ���
			{
				//printf("IPL_DEPTH_8S\n");//��֤��
				char *ps = (char*)(pres->imageData + y * pres->widthStep);
				for (int x = 0; x < pimg1->width; ++x)
				{
					//���㲢���ò�ֵ
					ps[x] = p1[x] - p2[x];
					//����С���ֵ
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
			case IPL_DEPTH_16S://IPL_DEPTH_16S�����Ļ����޷��Ŵ���
			{
				//printf("IPL_DEPTH_16S\n");//��֤��
				short *ps = (short*)(pres->imageData + y * pres->widthStep);
				for (int x = 0; x < pimg1->width; ++x)
				{
					//���㲢���ò�ֵ
					ps[x] = p1[x] - p2[x];
					//����С���ֵ
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
			case IPL_DEPTH_32S://IPL_DEPTH_32S�����Ļ����޷��Ŵ���
			{
				//printf("IPL_DEPTH_32S\n");//��֤��
				int *ps = (int*)(pres->imageData + y * pres->widthStep);
				for (int x = 0; x < pimg1->width; ++x)
				{
					//���㲢���ò�ֵ
					ps[x] = p1[x] - p2[x];
					//����С���ֵ
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
*���ܣ���ֵ��(������src��ʱֻ֧��IPL_DEPTH_8S��IPL_DEPTH_16S��IPL_DEPTH_32S�����)
*/
void mcvThreshold(IplImage *src, IplImage *dst, double threshold, double max_value, int type)
{
	for (int y = 0; y < src->height; ++y)
	{
		switch ((unsigned int)src->depth)
		{
			case IPL_DEPTH_8S://IPL_DEPTH_8S�����Ļ����޷��Ŵ���
			{
				//printf("IPL_DEPTH_8S\n");//��֤��
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
		case IPL_DEPTH_16S://IPL_DEPTH_16S�����Ļ����޷��Ŵ���
		{
			//printf("IPL_DEPTH_16S\n");//��֤��
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
		case IPL_DEPTH_32S://IPL_DEPTH_32S�����Ļ����޷��Ŵ���
		{
			//printf("IPL_DEPTH_32S\n");//��֤��
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
*���ܣ�����ͼ�������Сֵ��ѹ����0-255��Χ(src��ʱֻ֧��IPL_DEPTH_16S)
*������
*src������ͼ��mm����С���ֵ����
*����ֵ���Ҷ�ͼ��
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
//�����������򵥲���
void bkdminus_demo()
{
	const char *pSrcTitle = "src";
	const char *pImgTitle = "img";
	IplImage *psrc = cvLoadImage(string(respicrpath + "/bkd.jpg").c_str());
	IplImage *pimg = cvLoadImage(string(respicrpath + "/frame.jpg").c_str());

	objdetect_bkrndminus(pimg, psrc);

	cvNamedWindow(pSrcTitle);
	cvShowImage(pSrcTitle,psrc);

	cvNamedWindow(pImgTitle);
	cvShowImage(pImgTitle, pimg);

	cvWaitKey(0);

	cvDestroyWindow(pSrcTitle);
	cvReleaseImage(&psrc);
	cvDestroyWindow(pImgTitle);
	cvReleaseImage(&pimg);
}