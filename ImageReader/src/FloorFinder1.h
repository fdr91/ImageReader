/*
 * FloorFinder1.h
 *
 *  Created on: Apr 6, 2014
 *      Author: fdr91
 */

#ifndef FLOORFINDER1_H_
#define FLOORFINDER1_H_

#include "FloorFinder.h"
#include "ImageReader.h"

class FloorFinder1: public FloorFinder, private ImageReader {

private:

	int mMinimumRed;
	int mMaximumRed;
	int mRedGreenRangeMin;
	int mRedGreenRangeMax;
	int mRedBlueRangeMin;
	int mRedBlueRangeMax;

	int height;
	int width;

	void step1();
	void step2(cv::Mat& out);

	cv::Mat tmp;

public:

	int rectHeight;
	int rectWidth;
	cv::Mat getImg();
	void setRcDims(int w, int h);
	cv::Mat process();
	void showImage(int l=0);
	bool readImage(std::string name = "");
	FloorFinder1();
	FloorFinder1(std::string folder, std::string ext = "");
	FloorFinder1(std::string folder, std::string ext, int w, int h);
	virtual ~FloorFinder1();
};

#endif /* FLOORFINDER1_H_ */
