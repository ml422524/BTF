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
		out << i << " " << p[i] << std::endl;
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
		out << i << " " << p[i] << std::endl;
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
//-----------------------------------------------------
/*Below functions are used in SURF*/

static const int NCOLOURS = 8;
static const CvScalar COLOURS[] = { cvScalar(255,0,0), cvScalar(0,255,0),
cvScalar(0,0,255), cvScalar(255,255,0),
cvScalar(0,255,255), cvScalar(255,0,255),
cvScalar(255,255,255), cvScalar(0,0,0) };

//-------------------------------------------------------

//! Show the provided image and wait for keypress
void showImage(const IplImage *img)
{
	cvNamedWindow("Surf", CV_WINDOW_AUTOSIZE);
	cvShowImage("Surf", img);
	cvWaitKey(0);
}

//-------------------------------------------------------

//! Show the provided image in titled window and wait for keypress
void showImage(char *title, const IplImage *img)
{
	cvNamedWindow(title, CV_WINDOW_AUTOSIZE);
	cvShowImage(title, img);
	cvWaitKey(0);
}

//-------------------------------------------------------

//! Draw all the Ipoints in the provided vector
void drawIpoints(IplImage *img, std::vector<Ipoint> &ipts, int tailSize)
{
	Ipoint *ipt;
	float s, o;
	int r1, c1, r2, c2, lap;

	for (unsigned int i = 0; i < ipts.size(); i++)
	{
		ipt = &ipts.at(i);
		s = (2.5f * ipt->scale);
		o = ipt->orientation;
		lap = ipt->laplacian;
		r1 = fRound(ipt->y);
		c1 = fRound(ipt->x);
		c2 = fRound(s * cos(o)) + c1;
		r2 = fRound(s * sin(o)) + r1;

		if (o) // Green line indicates orientation
			cvLine(img, cvPoint(c1, r1), cvPoint(c2, r2), cvScalar(0, 255, 0));
		else  // Green dot if using upright version
			cvCircle(img, cvPoint(c1, r1), 1, cvScalar(0, 255, 0), -1);

		if (lap == 1)
		{ // Blue circles indicate dark blobs on light backgrounds
			cvCircle(img, cvPoint(c1, r1), fRound(s), cvScalar(255, 0, 0), 1);
		}
		else if (lap == 0)
		{ // Red circles indicate light blobs on dark backgrounds
			cvCircle(img, cvPoint(c1, r1), fRound(s), cvScalar(0, 0, 255), 1);
		}
		else if (lap == 9)
		{ // Red circles indicate light blobs on dark backgrounds
			cvCircle(img, cvPoint(c1, r1), fRound(s), cvScalar(0, 255, 0), 1);
		}

		// Draw motion from ipoint dx and dy
		if (tailSize)
		{
			cvLine(img, cvPoint(c1, r1),
				cvPoint(int(c1 + ipt->dx*tailSize), int(r1 + ipt->dy*tailSize)),
				cvScalar(255, 255, 255), 1);
		}
	}
}

//-------------------------------------------------------

//! Draw a single feature on the image
void drawIpoint(IplImage *img, Ipoint &ipt, int tailSize)
{
	float s, o;
	int r1, c1, r2, c2, lap;

	s = (2.5f * ipt.scale);
	o = ipt.orientation;
	lap = ipt.laplacian;
	r1 = fRound(ipt.y);
	c1 = fRound(ipt.x);

	// Green line indicates orientation
	if (o) // Green line indicates orientation
	{
		c2 = fRound(s * cos(o)) + c1;
		r2 = fRound(s * sin(o)) + r1;
		cvLine(img, cvPoint(c1, r1), cvPoint(c2, r2), cvScalar(0, 255, 0));
	}
	else  // Green dot if using upright version
		cvCircle(img, cvPoint(c1, r1), 1, cvScalar(0, 255, 0), -1);

	if (lap >= 0)
	{ // Blue circles indicate light blobs on dark backgrounds
		cvCircle(img, cvPoint(c1, r1), fRound(s), cvScalar(255, 0, 0), 1);
	}
	else
	{ // Red circles indicate light blobs on dark backgrounds
		cvCircle(img, cvPoint(c1, r1), fRound(s), cvScalar(0, 0, 255), 1);
	}

	// Draw motion from ipoint dx and dy
	if (tailSize)
	{
		cvLine(img, cvPoint(c1, r1),
			cvPoint(int(c1 + ipt.dx*tailSize), int(r1 + ipt.dy*tailSize)),
			cvScalar(255, 255, 255), 1);
	}
}

//-------------------------------------------------------

//! Draw a single feature on the image
void drawPoint(IplImage *img, Ipoint &ipt)
{
	float s, o;
	int r1, c1;

	s = 3;
	o = ipt.orientation;
	r1 = fRound(ipt.y);
	c1 = fRound(ipt.x);

	cvCircle(img, cvPoint(c1, r1), fRound(s), COLOURS[ipt.clusterIndex%NCOLOURS], -1);
	cvCircle(img, cvPoint(c1, r1), fRound(s + 1), COLOURS[(ipt.clusterIndex + 1) % NCOLOURS], 2);

}

//-------------------------------------------------------

//! Draw a single feature on the image
void drawPoints(IplImage *img, std::vector<Ipoint> &ipts)
{
	float s, o;
	int r1, c1;

	for (unsigned int i = 0; i < ipts.size(); i++)
	{
		s = 3;
		o = ipts[i].orientation;
		r1 = fRound(ipts[i].y);
		c1 = fRound(ipts[i].x);

		cvCircle(img, cvPoint(c1, r1), fRound(s), COLOURS[ipts[i].clusterIndex%NCOLOURS], -1);
		cvCircle(img, cvPoint(c1, r1), fRound(s + 1), COLOURS[(ipts[i].clusterIndex + 1) % NCOLOURS], 2);
	}
}

//-------------------------------------------------------

//! Draw descriptor windows around Ipoints in the provided vector
void drawWindows(IplImage *img, std::vector<Ipoint> &ipts)
{
	Ipoint *ipt;
	float s, o, cd, sd;
	int x, y;
	CvPoint2D32f src[4];

	for (unsigned int i = 0; i < ipts.size(); i++)
	{
		ipt = &ipts.at(i);
		s = (10 * ipt->scale);
		o = ipt->orientation;
		y = fRound(ipt->y);
		x = fRound(ipt->x);
		cd = cos(o);
		sd = sin(o);

		src[0].x = sd*s + cd*s + x;   src[0].y = -cd*s + sd*s + y;
		src[1].x = sd*s + cd*-s + x;  src[1].y = -cd*s + sd*-s + y;
		src[2].x = sd*-s + cd*-s + x; src[2].y = -cd*-s + sd*-s + y;
		src[3].x = sd*-s + cd*s + x;  src[3].y = -cd*-s + sd*s + y;

		if (o) // Draw orientation line
			cvLine(img, cvPoint(x, y),
				cvPoint(fRound(s*cd + x), fRound(s*sd + y)), cvScalar(0, 255, 0), 1);
		else  // Green dot if using upright version
			cvCircle(img, cvPoint(x, y), 1, cvScalar(0, 255, 0), -1);


		// Draw box window around the point
		cvLine(img, cvPoint(fRound(src[0].x), fRound(src[0].y)),
			cvPoint(fRound(src[1].x), fRound(src[1].y)), cvScalar(255, 0, 0), 2);
		cvLine(img, cvPoint(fRound(src[1].x), fRound(src[1].y)),
			cvPoint(fRound(src[2].x), fRound(src[2].y)), cvScalar(255, 0, 0), 2);
		cvLine(img, cvPoint(fRound(src[2].x), fRound(src[2].y)),
			cvPoint(fRound(src[3].x), fRound(src[3].y)), cvScalar(255, 0, 0), 2);
		cvLine(img, cvPoint(fRound(src[3].x), fRound(src[3].y)),
			cvPoint(fRound(src[0].x), fRound(src[0].y)), cvScalar(255, 0, 0), 2);

	}
}

//-------------------------------------------------------

// Draw the FPS figure on the image (requires at least 2 calls)
void drawFPS(IplImage *img)
{
	static int counter = 0;
	static clock_t t;
	static float fps;
	char fps_text[20];
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, 1.0, 1.0, 0, 2);

	// Add fps figure (every 10 frames)
	if (counter > 10)
	{
		fps = (10.0f / (clock() - t) * CLOCKS_PER_SEC);
		t = clock();
		counter = 0;
	}

	// Increment counter
	++counter;

	// Get the figure as a string
	sprintf(fps_text, "FPS: %.2f", fps);

	// Draw the string on the image
	cvPutText(img, fps_text, cvPoint(10, 25), &font, cvScalar(255, 255, 0));
}

//-------------------------------------------------------

//! Save the SURF features to file
void saveSurf(char *filename, std::vector<Ipoint> &ipts)
{
	ofstream outfile(filename);

	// output descriptor length
	outfile << "64\n";
	outfile << ipts.size() << "\n";

	// create output line as:  scale  x  y  des
	for (unsigned int i = 0; i < ipts.size(); i++)
	{
		outfile << ipts.at(i).scale << "  ";
		outfile << ipts.at(i).x << " ";
		outfile << ipts.at(i).y << " ";
		outfile << ipts.at(i).orientation << " ";
		outfile << ipts.at(i).laplacian << " ";
		outfile << ipts.at(i).scale << " ";
		for (int j = 0; j<64; j++)
			outfile << ipts.at(i).descriptor[j] << " ";

		outfile << "\n";
	}

	outfile.close();
}

//-------------------------------------------------------

//! Load the SURF features from file
void loadSurf(char *filename, std::vector<Ipoint> &ipts)
{
	int descriptorLength, count;
	ifstream infile(filename);

	// clear the ipts vector first
	ipts.clear();

	// read descriptor length/number of ipoints
	infile >> descriptorLength;
	infile >> count;

	// for each ipoint
	for (int i = 0; i < count; i++)
	{
		Ipoint ipt;

		// read vals
		infile >> ipt.scale;
		infile >> ipt.x;
		infile >> ipt.y;
		infile >> ipt.orientation;
		infile >> ipt.laplacian;
		infile >> ipt.scale;

		// read descriptor components
		for (int j = 0; j < 64; j++)
			infile >> ipt.descriptor[j];

		ipts.push_back(ipt);

	}
}
/*Above functions are used in SURF*/
//----------------------------------------------------------