#ifndef IPOINT_H
#define IPOINT_H

#include <vector>
#include <cmath>
#include "cv.h"
//-------------------------------------------------------

class Ipoint; // Pre-declaration
typedef std::vector<Ipoint> IpVec;
typedef std::vector<std::pair<Ipoint, Ipoint> > IpPairVec;

//-------------------------------------------------------
class Ipoint {

public:

  //! Destructor
  ~Ipoint() {};

  //! Constructor
  Ipoint() : orientation(0) {};

  //! Gets the distance in descriptor space between Ipoints
  float operator-(const Ipoint &rhs)
  {
    float sum=0.f;
    for(int i=0; i < 64; ++i)
      sum += (this->descriptor[i] - rhs.descriptor[i])*(this->descriptor[i] - rhs.descriptor[i]);
    return sqrt(sum);
  };

  //! Coordinates of the detected interest point
  float x, y;

  //! Detected scale
  float scale;

  //! Orientation measured anti-clockwise from +ve x-axis
  float orientation;

  //! Sign of laplacian for fast matching purposes
  int laplacian;

  //! Vector of descriptor components
  float descriptor[64];

  //! Placeholds for point motion (can be used for frame to frame motion analysis)
  float dx, dy;

  //! Used to store cluster index
  int clusterIndex;
};

//! Ipoint operations
void getMatches(IpVec &ipts1, IpVec &ipts2, IpPairVec &matches,float threshold = 0.65);//theshold越小，匹配越严格。
int translateCorners(IpPairVec &matches, const CvPoint src_corners[4], CvPoint dst_corners[4]);

#endif
