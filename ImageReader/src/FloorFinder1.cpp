/*
 * FloorFinder1.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: fdr91
 */

#include "FloorFinder1.h"
#include "log.h"
#include <stdio.h>;

using namespace std;

FloorFinder1::FloorFinder1() {
	mMinimumRed =255;
	mMaximumRed = 0;
	mRedGreenRangeMin = 255;
	mRedGreenRangeMax = 0;
	mRedBlueRangeMin = 255;
	mRedBlueRangeMax = 0;
}


FloorFinder1::FloorFinder1(string folder, string ext): ImageReader(folder, ext) {
	mMinimumRed =255;
	mMaximumRed = 0;
	mRedGreenRangeMin = 255;
	mRedGreenRangeMax = 0;
	mRedBlueRangeMin = 255;
	mRedBlueRangeMax = 0;
}

FloorFinder1::FloorFinder1(string folder, string ext, int w, int h): ImageReader(folder, ext) {
	mMinimumRed =255;
	mMaximumRed = 0;
	mRedGreenRangeMin = 255;
	mRedGreenRangeMax = 0;
	mRedBlueRangeMin = 255;
	mRedBlueRangeMax = 0;
	rectWidth=w;
	rectHeight=h;
}

void FloorFinder1::setRcDims(int w, int h)
{
	this->rectWidth=w;
	this->rectHeight=h;
}


bool FloorFinder1::readImage(string name) {

		bool ret = true;

		if(name.compare("")==0)
		{
			ret = this->readNext();
			height = img.rows;
			width = img.cols;

		}
		else
		{
			img = cv::imread(name);
			height = img.rows;
			width = img.cols;
		}
		if(img.data!=NULL)
		{
			if((img.cols>800 || img.rows > 800))
				cv::resize(img, img, cv::Size(img.cols/4,img.rows/4));
			height = img.rows;
			width = img.cols;
		}
		else
			ret=false;
		return ret;
}

cv::Mat FloorFinder1::getImg()
{
	return img.clone();
}

void FloorFinder1::showImage(int l)
{
	show(l);
}

void setPixels(cv::Mat& img, cv::Vec3b bgr, cv::Point pos)
{
	img.at<cv::Vec3b>(pos.y, pos.x) = bgr;
}

void FloorFinder1::step1()
{
	DBG("step 1: start");

	mMinimumRed =255;
	mMaximumRed = 0;
	mRedGreenRangeMin = 255;
	mRedGreenRangeMax = 0;
	mRedBlueRangeMin = 255;
	mRedBlueRangeMax = 0;

	int recUp = height-rectHeight;
	int recDown = height;
	int recLeft =  (width-rectWidth-1)/2;
	int recRight = (width+rectWidth-1)/2;

	cv::Mat testRect = img.rowRange(recUp,recDown).colRange(recLeft,recRight).clone();


	for (int x = recLeft; x < recRight; x++) {
		for (int y = recUp; y < recDown; y++) {


			cv::Vec3b bgrPixel = img.at<cv::Vec3b>(y,x);

			unsigned char blue = bgrPixel.val[0];
			unsigned char green = bgrPixel.val[1];
			unsigned char red = bgrPixel.val[2];

			if (red == 0 && green == 0 && blue == 0) {
				continue;
			}

			//Find t h r e s h o l d s
			if (red < mMinimumRed) {
				mMinimumRed = red;
			}

			if (red > mMaximumRed) {
				mMaximumRed = red;
			}

			int	redGreenRange = red-green;
			int redBlueRange = red-blue;

			if (redGreenRange < mRedGreenRangeMin) {
				mRedGreenRangeMin = redGreenRange;
			}

			if (redGreenRange > mRedGreenRangeMax) {
				mRedGreenRangeMax = redGreenRange;
			}

			if ( redBlueRange < mRedBlueRangeMin) {
				mRedBlueRangeMin = redBlueRange;
			}

			if ( redBlueRange > mRedBlueRangeMax) {
				mRedBlueRangeMax = redBlueRange;
			}
		}
	}
	DBG("step 1: end");
}

void FloorFinder1::step2(cv::Mat& out)
{
	DBG("step 2: start");
	int totalPixels = 0;
	out = img.clone();

	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {
			cv::Vec3b bgrPixel = img.at<cv::Vec3b>(y, x);
			unsigned char blue = bgrPixel.val[0];
			unsigned char green = bgrPixel.val[1];
			unsigned char red = bgrPixel.val[2];
			int redGreen = red - green;
			int redBlue = red - blue;

			if ((red > mMinimumRed && red < mMaximumRed)
					&& (redGreen >= mRedGreenRangeMin
							&& redGreen <= mRedGreenRangeMax)
					&& (redBlue >= mRedBlueRangeMin
							&& redBlue <= mRedBlueRangeMax)) {
				// p i x e l i s w i t h i n f l o o r r a ng e s e t to w h i t e/*/
				setPixels(out, cv::Vec3b(255, 255, 255), cv::Point(x, y));
				++totalPixels;
			} else {
				setPixels(out, cv::Vec3b(0, 0, 0), cv::Point(x, y));
			}
		}
	}
	DBG("step 2: end");
}

cv::Mat FloorFinder1::process()
{
	DBG("FloorFinder1::process");

	step1();

	cv::Mat out;
	step2(out);

	int recUp = height-rectHeight;
	int recDown = height;
	int recLeft =  (width-rectWidth-1)/2;
	int recRight = (width+rectWidth-1)/2;




	/*cv::imshow("Out", out);
	cv::waitKey(1000);*/
	return out;
	/*for ( auto it=FilteredLines.begin(); it!=FilteredLines.end( ); ++it)
	{
		CvPoint polyPoint s[4 ] ;

		// Fi r s t point i s to the l e f t o f the r i g h t point
		 if ((*it)[0 ] <= (*it)[2])
		 {
		 polyPoint s[0]=cvPoint ((*it)[0]0, 0);
		 polyPoint s[1]=cvPoint ((*it)[2]0, 0);
		 polyPoint s[2]=cvPoint ((*it)[2], (∗it)[3]);
		 polyPoint s[3]=cvPoint ((*it)[0], (∗it)[1]);
		 }
		 else // Fi r s t point i s to the r i g h t o f the r i g h t point
		 {
		 polyPoint s[0]=cvPoint ((*it)[2],0);
		 polyPoint s[1]=cvPoint ((*it)[0],0);
		 polyPoint s[2]=cvPoint ((*it)[0],(∗it)[1]);
		 polyPoint s[3]=cvPoint ((*it)[2],(∗it)[3]);
		 }

		 cvFillConvexPoly( inImage , &polyPoint s[0 ] , 4 , cvScalar (0, 0, 0));
	 }*/
}

FloorFinder1::~FloorFinder1() {
	// TODO Auto-generated destructor stub
}

