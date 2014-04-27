/*
 * ImageReader.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: fdr91
 */

using namespace std;

#include "ImageReader.h"
#include "log.h"
#include "helpers.h"
#include <string>
#include <boost/lexical_cast.hpp>

void pprint(std::string s)
{
	DBG(s);
}

ImageReader::ImageReader(std::string folder, std::string ext)
{
	window = "Image Reader";
	this->folder=folder;
	this->ext = ext;
	createFileList();
	iter = files.begin();

}

ImageReader::ImageReader() {
	this->folder = "./";
	this->ext="";
	window = "Image Reader";
}

bool ImageReader::readNext()
{
	if(iter!=files.end())
	{
		DBG("Read next image");
		img = cv::imread(*iter++, CV_LOAD_IMAGE_COLOR);
		return true;
	}
	else
	{
		img.release();
		return false;
	}
}

void ImageReader::show(int l)
{
	DBG("Show image");
	cv::imshow(window, img);
	cv::waitKey(l);
}

void ImageReader::createFileList(){
	vector<string> s;
	getdir(folder, s);
	int l = ext.length();

	for(vector<string>::iterator it = s.begin() ; it != s.end(); ++it)
	{
		if((*it).compare("..") == 0 || (*it).compare(".") == 0)
			continue;

		if(((*it).substr((*it).length()-l, l).compare(ext))==0)
			files.push_back(folder+*it);
	}
}

/*void ImageReader::setOptions(std::string folder, std::string extenstion)
{
	this->folder=folder;
	this->ext = extenstion;
	createFileList();
	iter = files.begin();
}*/

ImageReader::~ImageReader() {
	// TODO Auto-generated destructor stub
}

