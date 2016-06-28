#include "integral.h"

/*
*���ܣ�Convert image to single channel 32F
*������
*imgͼ��
*����ֵ��32-bit float��[0,1]��Χ��ͼ��
*/
IplImage *imtofloat32(const IplImage *img)
{
	// Check we have been supplied a non-null img pointer
	if (!img) 
	{
		printf("Unable to create grayscale image.  No image supplied\n"); exit(0);
	}

	IplImage *gray8, *gray32;

	gray32 = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);

	if (img->nChannels == 1)//����ǻҶ�ͼ�񣬿��Բ��ÿ�����ֱ����ԭͼ�����Ż���
		gray8 = (IplImage *)cvClone(img);
	else 
	{
		gray8 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		cvCvtColor(img, gray8, CV_BGR2GRAY);
	}

	cvConvertScale(gray8, gray32, 1.0 / 255.0, 0);

	cvReleaseImage(&gray8);

	return gray32;
}
/*
*���ܣ��������ͼ(wxh)
*������
*img������ͼ��(wxh)
*����ֵ������ͼ��(wxh),32-bit floating point����ͨ��
*��������ͼ��I(x,y),ԭͼ��f(x,y),I(2,1)=f(0,0)+f(1,0)+f(2,0)+f(0,1)+f(1,1)+f(2,1).
//x<=x0,y<=y0
*/
IplImage *Integral(IplImage *source)
{
	// convert the image to single channel 32f��[0,1]
	IplImage *img = imtofloat32(source);
	IplImage *int_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);

	// set up variables for data access
	int height = img->height;
	int width = img->width;
	int step = img->widthStep / sizeof(float);
	float *data = (float *)img->imageData;
	float *i_data = (float *)int_img->imageData;

	// first row only
	float rs = 0.0f;
	for (int j = 0; j < width; j++)
	{
		rs += data[j];
		i_data[j] = rs;
	}

	// remaining cells are sum above and to the left
	for (int i = 1; i < height; ++i)
	{
		rs = 0.0f;
		for (int j = 0; j < width; ++j)
		{
			rs += data[i*step + j];
			i_data[i*step + j] = rs + i_data[(i - 1)*step + j];
		}
	}

	// release the gray image
	cvReleaseImage(&img);

	// return the integral image
	return int_img;
}