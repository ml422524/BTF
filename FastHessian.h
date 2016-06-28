#ifndef _FASTHESSIAN_H_
#define _FASTHESSIAN_H_

#include <vector>
#include <memory.h>
#include "cv.h"
#include "ipoint.h"
#include "integral.h"

/*start of ResponseLayer*/
class ResponseLayer
{
public:

	int width, height, step, filter;
	float *responses;
	unsigned char *laplacian;

	ResponseLayer(int width, int height, int step, int filter)
	{
		assert(width > 0 && height > 0);

		this->width = width;
		this->height = height;
		this->step = step;//采样步进
		this->filter = filter;//滤波器尺寸

		responses = new float[width*height];
		laplacian = new unsigned char[width*height];

		memset(responses, 0, sizeof(float)*width*height);
		memset(laplacian, 0, sizeof(unsigned char)*width*height);
	}

	~ResponseLayer()
	{
		if (responses) delete[] responses;
		if (laplacian) delete[] laplacian;
	}

	inline unsigned char getLaplacian(unsigned int row, unsigned int column)
	{
		return laplacian[row * width + column];
	}

	inline unsigned char getLaplacian(unsigned int row, unsigned int column, ResponseLayer *src)
	{
		int scale = this->width / src->width;

		return laplacian[(scale * row) * width + (scale * column)];
	}

	inline float getResponse(unsigned int row, unsigned int column)
	{
		return responses[row * width + column];
	}

	inline float getResponse(unsigned int row, unsigned int column, ResponseLayer *src)
	{
		int scale = this->width / src->width;
		return responses[(scale * row) * width + (scale * column)];
	}
};
/*end of ResponseLayer*/

static const int OCTAVES = 5;
static const int INTERVALS = 4;
static const int INIT_SAMPLE = 2;
static const float THRES = 0.0004f;

class FastHessian {

public:

	//! Constructor with image
	FastHessian(IplImage *img,
		std::vector<Ipoint> &ipts,
		const int octaves = OCTAVES,
		const int intervals = INTERVALS,
		const int init_sample = INIT_SAMPLE,
		const float thres = THRES);

	//! Save the parameters
	void saveParameters(const int octaves,
		const int intervals,
		const int init_sample,
		const float thres);

	//! Set or re-set the integral image source
	void setIntImage(IplImage *img);

	//! Find the image features and write into vector of features
	void getIpoints();

private:

	//---------------- Private Functions -----------------//

	//! Build map of DoH responses
	void buildResponseMap();

	//! Calculate DoH responses for supplied layer
	void buildResponseLayer(ResponseLayer *r);

	//! 3x3x3 Extrema test
	int isExtremum(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b);

	//! Interpolation functions - adapted from Lowe's SIFT implementation
	void interpolateExtremum(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b);
	void interpolateStep(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b,
		double* xi, double* xr, double* xc);

	CvMat* deriv3D(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b);
	CvMat* hessian3D(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b);

	//---------------- Private Variables -----------------//

	//! Pointer to the integral Image, and its attributes 
	IplImage *img;
	int i_width, i_height;

	//! Reference to vector of features passed from outside 
	std::vector<Ipoint> &ipts;

	//! Number of Octaves
	int octaves;

	//! Number of Intervals per octave
	int intervals;

	//! Initial sampling step for Ipoint detection
	int init_sample;

	//! Threshold value for blob resonses
	float thresh;

	//! Response stack of determinant of hessian values
	std::vector<ResponseLayer *> responseMap;


};


#endif
