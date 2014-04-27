/*
 * ImageReader.h
 *
 *  Created on: Apr 6, 2014
 *      Author: fdr91
 */

#ifndef IMAGEREADER_H_
#define IMAGEREADER_H_

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

class ImageReader {
protected:
	cv::Mat img;

private:
	std::string folder;
	std::string ext;
	std::vector<std::string>::iterator iter;
	std::vector<std::string> files;
	std::string window;

	void createFileList();

public:
	ImageReader();
	ImageReader(std::string folder, std::string extenstion = "");

	//void setOptions(std::string folder, std::string extenstion = "");
	bool readNext();
	virtual void show(int l=0);
	~ImageReader();
};

#endif /* IMAGEREADER_H_ */
