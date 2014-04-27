/*
 * FloorFinder.h
 *
 *  Created on: Apr 6, 2014
 *      Author: fdr91
 */

#ifndef FLOORFINDER_H_
#define FLOORFINDER_H_

#include <string>
#include "ImageReader.h"


class FloorFinder{

public:

	virtual cv::Mat process() = 0;
	virtual void showImage(int l=0) = 0;
	virtual bool readImage(std::string name = "") = 0;
	FloorFinder();
	virtual ~FloorFinder();
};

#endif /* FLOORFINDER_H_ */
