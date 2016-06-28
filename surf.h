#ifndef _SURF_H_
#define _SURF_H_

#include <cv.h>
#include <highgui.h>

#include "integral.h"
#include "FastHessian.h"
#include "surf.h"
#include "ipoint.h"
#include "function.h"

class Surf {

public:

	//! Standard Constructor (img is an integral image)
	Surf(IplImage *img, IpVec &ipts);

	//! Describe all features in the supplied vector
	void getDescriptors(bool bUpright = false);

private:

	//---------------- Private Functions -----------------//

	//! Assign the current Ipoint an orientation
	void getOrientation();

	//! Get the descriptor. See Agrawal ECCV 08
	void getDescriptor(bool bUpright = false);

	//! Calculate the value of the 2d gaussian at x,y
	inline float gaussian(int x, int y, float sig);
	inline float gaussian(float x, float y, float sig);

	//! Calculate Haar wavelet responses in x and y directions
	inline float haarX(int row, int column, int size);
	inline float haarY(int row, int column, int size);

	//! Get the angle from the +ve x-axis of the vector given by [X Y]
	float getAngle(float X, float Y);


	//---------------- Private Variables -----------------//

	//! Integral image where Ipoints have been detected
	IplImage *img;

	//! Ipoints vector
	IpVec &ipts;

	//! Index of current Ipoint in the vector
	int index;
};

//! Library function builds vector of described interest points
inline void surfDetDes(IplImage *img,  /* image to find Ipoints in */
	std::vector<Ipoint> &ipts, /* reference to vector of Ipoints */
	bool upright = false, /* run in rotation invariant mode? */
	int octaves = OCTAVES, /* number of octaves to calculate */
	int intervals = INTERVALS, /* number of intervals per octave */
	int init_sample = INIT_SAMPLE, /* initial sampling step */
	float thres = THRES /* blob response threshold */)
{
	// Create integral-image representation of the image
	IplImage *int_img = Integral(img);

	// Create Fast Hessian Object
	FastHessian fh(int_img, ipts, octaves, intervals, init_sample, thres);

	// Extract interest points and store in vector ipts
	fh.getIpoints();

	// Create Surf Descriptor Object
	Surf des(int_img, ipts);

	// Extract the descriptors for the ipts
	des.getDescriptors(upright);

	// Deallocate the integral image
	cvReleaseImage(&int_img);
}


//! Library function builds vector of interest points
inline void surfDet(IplImage *img,  /* image to find Ipoints in */
	std::vector<Ipoint> &ipts, /* reference to vector of Ipoints */
	int octaves = OCTAVES, /* number of octaves to calculate */
	int intervals = INTERVALS, /* number of intervals per octave */
	int init_sample = INIT_SAMPLE, /* initial sampling step */
	float thres = THRES /* blob response threshold */)
{
	// Create integral image representation of the image
	IplImage *int_img = Integral(img);

	// Create Fast Hessian Object
	FastHessian fh(int_img, ipts, octaves, intervals, init_sample, thres);

	// Extract interest points and store in vector ipts
	fh.getIpoints();

	// Deallocate the integral image
	cvReleaseImage(&int_img);
}

//! Library function describes interest points in vector
inline void surfDes(IplImage *img,  /* image to find Ipoints in */
	std::vector<Ipoint> &ipts, /* reference to vector of Ipoints */
	bool upright = false) /* run in rotation invariant mode? */
{
	// Create integral image representation of the image
	IplImage *int_img = Integral(img);

	// Create Surf Descriptor Object
	Surf des(int_img, ipts);

	// Extract the descriptors for the ipts
	des.getDescriptors(upright);

	// Deallocate the integral image
	cvReleaseImage(&int_img);
}


#endif
