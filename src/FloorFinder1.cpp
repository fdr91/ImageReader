/*
 * FloorFinder1.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: fdr91
 */

#include "FloorFinder1.h"
#include "log.h"
#include "helpers.h"
#include <iostream>

//#define CONFIG_FILE "./conf.xml"
#define ROOT "ImageReader"

#define STANDART_WIDTH 640
#define STANDART_HEIGHT 480

using namespace std;
using namespace cv;

FloorFinder1::FloorFinder1() {
	mMinimumRed = 255;
	mMaximumRed = 0;
	mRedGreenRangeMin = 255;
	mRedGreenRangeMax = 0;
	mRedBlueRangeMin = 255;
	mRedBlueRangeMax = 0;
	cT1 = 1;
	cT2 = 1;
}

FloorFinder1::FloorFinder1(string folder, string ext, int w, int h) :
		ImageReader(folder, ext) {
	mMinimumRed = 255;
	mMaximumRed = 0;
	mRedGreenRangeMin = 255;
	mRedGreenRangeMax = 0;
	mRedBlueRangeMin = 255;
	mRedBlueRangeMax = 0;
	cT1 = 1;
	cT2 = 1;
	this->rectHeight = h;
	this->rectWidth = w;
}

FloorFinder1::FloorFinder1(string configFile, int i) :
		ImageReader(readConfigs(configFile, ROOT, "folder"),
				readConfigs(configFile, ROOT, "extension")) {
	string sRectHeight = readConfigs(configFile, ROOT, "rect_height");
	string sRectWidth = readConfigs(configFile, ROOT, "rect_width");
	rectHeight = atoi(sRectHeight.data());
	rectWidth = atoi(sRectWidth.data());
}

void FloorFinder1::setRcDims(int w, int h) {
	this->rectWidth = w;
	this->rectHeight = h;
}

Size2i getStandartSizes(cv::Size2i size) {
	Size2i s(size);
	/*int width = s.width;
	 int height = s.height;*/
	int i = 2;
	while (s.width / i > STANDART_WIDTH)
		i++;
	s.width /= i;
	i = 2;
	while (s.height / i > STANDART_HEIGHT)
		i++;
	s.height /= i;
	if (s.width > STANDART_WIDTH || s.height > STANDART_HEIGHT) {
		cerr << "Bad resolution!!" << endl;
		throw 1;
	}
	return s;
}

bool FloorFinder1::readImage(string name) {

	bool ret = true;

	if (name.compare("") == 0) {
		ret = this->readNext();
		height = img.rows;
		width = img.cols;

	} else {
		img = cv::imread(name);
		height = img.rows;
		width = img.cols;
	}
	if (img.data != NULL) {
		if ((img.cols > STANDART_WIDTH || img.rows > STANDART_HEIGHT)) {
			Size2i s = getStandartSizes(Size2i(img.cols, img.rows));
			cv::resize(img, img, s);
		}
		height = img.rows;
		width = img.cols;
	} else
		ret = false;
	return ret;
}

cv::Mat FloorFinder1::getImg() {
	return img.clone();
}

void FloorFinder1::showImage(int l) {
	show(l);
}

void setPixels(cv::Mat& img, cv::Vec3b bgr, cv::Point pos) {
	img.at<cv::Vec3b>(pos.y, pos.x) = bgr;
}

void FloorFinder1::step1() {
	mMinimumRed = 255;
	mMaximumRed = 0;
	mRedGreenRangeMin = 255;
	mRedGreenRangeMax = 0;
	mRedBlueRangeMin = 255;
	mRedBlueRangeMax = 0;

	int recUp = height - rectHeight;
	int recDown = height;
	int recLeft = (width - rectWidth - 1) / 2;
	int recRight = (width + rectWidth - 1) / 2;

	cv::Mat testRect =
			img.rowRange(recUp, recDown).colRange(recLeft, recRight).clone();

	for (int x = recLeft; x < recRight; x++) {
		for (int y = recUp; y < recDown; y++) {

			cv::Vec3b bgrPixel = img.at<cv::Vec3b>(y, x);

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

			int redGreenRange = red - green;
			int redBlueRange = red - blue;

			if (redGreenRange < mRedGreenRangeMin) {
				mRedGreenRangeMin = redGreenRange;
			}

			if (redGreenRange > mRedGreenRangeMax) {
				mRedGreenRangeMax = redGreenRange;
			}

			if (redBlueRange < mRedBlueRangeMin) {
				mRedBlueRangeMin = redBlueRange;
			}

			if (redBlueRange > mRedBlueRangeMax) {
				mRedBlueRangeMax = redBlueRange;
			}
		}
	}
}

void FloorFinder1::step2(/*cv::Mat& out*/) {
	int totalPixels = 0;
	out1 = img.clone();

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
				setPixels(out1, cv::Vec3b(255, 255, 255), cv::Point(x, y));
				++totalPixels;
			} else {
				setPixels(out1, cv::Vec3b(0, 0, 0), cv::Point(x, y));
			}
		}
	}
}

void FloorFinder1::step3() {
	out2 = out1.clone();
	vector<Vec4i> lines; // = new vector();
	cannyAndHough(img, lines);
	poly(out2, lines);
}

void FloorFinder1::cannyAndHough(cv::Mat& src, vector<Vec4i>& lines) {
	//void Canny(InputArray image, OutputArray edges, double threshold1, double threshold2, int apertureSize=3, bool L2gradient=false )
	//
	Mat src_gray;
	cvtColor(src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(5, 5));
	Canny(src_gray, src_gray, cT1, cT2);
	//vector<Vec4i> lines;
	HoughLinesP(src_gray, lines, 1, CV_PI / 180, 80, 30, 10);
	out3 = img.clone();
	for (size_t i = 0; i < lines.size(); i++) {
		line(out3, Point(lines[i][0], lines[i][1]),
				Point(lines[i][2], lines[i][3]), Scalar(0, 0, 255), 3, 8);
	}
}

void FloorFinder1::poly(Mat& src, vector<Vec4i>& lines) {
	//for ( auto it=lines.begin(); it!=lines.end( ); ++it)
	for (unsigned int i = 0; i < lines.size(); i++) {
		Vec4i line = lines.at(i);
		Point polyPoints[4];

		// Fi r s t point i s to the l e f t o f the r i g h t point
		if (line[0] <= line[2]) {
			polyPoints[0] = Point(line[0], 0);
			polyPoints[1] = Point(line[2], 0);
			polyPoints[2] = Point(line[2], line[3]);
			polyPoints[3] = Point(line[0], line[1]);
		} else // Fi r s t point i s to the r i g h t o f the r i g h t point
		{
			polyPoints[0] = Point(line[2], 0);
			polyPoints[1] = Point(line[0], 0);
			polyPoints[2] = Point(line[0], line[1]);
			polyPoints[3] = Point(line[2], line[3]);
		}

		fillConvexPoly(src, polyPoints, 4, Scalar(0, 0, 0));
	}
}

cv::Mat FloorFinder1::process() {
	step1();
	step2();
	step3();
	return out1;
}

double cTmax = 300;

void FloorFinder1::createTrackBarsAndWindows() {

	cv::namedWindow("original");
	cv::namedWindow("floor");
	cv::namedWindow("canny");

	cv::createTrackbar("height", "original", &rectHeight, 255, NULL);
	cv::createTrackbar("width", "original", &rectWidth, 255, NULL);

	cv::createTrackbar("threshold1", "original", &cT1, cTmax, NULL);
	cv::createTrackbar("threshold2", "original", &cT2, cTmax, NULL);
}

void FloorFinder1::loop() {
	createTrackBarsAndWindows();
	Mat src;
	char c = 0;
	char cl = 0;
	if (readImage()) {
		while (c != 27) {
			src = getImg();
			if (img.rows > 0) {
				process();
				int x = (img.cols - rectWidth - 1) / 2;
				int y = img.rows - rectHeight;
				Rect r(x, y, rectWidth, rectHeight);
				imshow("canny", out1);
				imshow("floor", out2);
				rectangle(out3, r, cv::Scalar(255, 0, 0));
				imshow("original", out3);
				c = cv::waitKey(33);
			}

			if (c == cl) {
			} else if (c == ' ') {
				DBG("Space -- Read next image...");
				bool f = readImage();
				if (!f)
					break;
			}
			cl = c;
		}
	} else {
		cerr<<"Can't read image!"<<endl;
		throw 2;
	}
	DBG("Exit OK");
}

FloorFinder1::~FloorFinder1() {
	// TODO Auto-generated destructor stub
}

