#include "ImageReader.h"
#include "helpers.h"
#include "log.h"

#include <boost/lexical_cast.hpp>
#include <tinyxml.h>
#include "FloorFinder.h"
#include "FloorFinder1.h"


#define CONFIG_FILE "./conf.xml"
#define ROOT "ImageReader"


using namespace std;
using namespace cv;

string folder;
string ext;

int rectHeight;
int rectWidth;

double cTmax = 100;

int cT1=1;
int cT2=1;

int i = 0;
int p=1;


void init()
{
	folder = readConfigs(CONFIG_FILE, ROOT, "folder");
	ext =  readConfigs(CONFIG_FILE, ROOT, "extension");
	string sRectHeight =  readConfigs(CONFIG_FILE, ROOT, "rect_height");
	string sRectWidth =  readConfigs(CONFIG_FILE, ROOT, "rect_width");
	rectHeight = atoi(sRectHeight.data());
	rectWidth = atoi(sRectWidth.data());
	DBG(string("Folder is ")+folder);
	DBG(string("Extension is ")+ext);
}

void trackBarCbHeight(int param, void *v=0)
{
	rectHeight = param > 0 ? param : 1;
}

void trackBarCbWidth(int param, void *v=0)
{
	rectWidth = param > 0 ? param : 1;
}

void trackBarCT1(int param, void *v=0)
{
	//cT1 = param;// > 0 ? param : 1;
}

void trackBarCT2(int param, void *v=0)
{
	//cT2 = param;// > 0 ? param : 1;
}

void cannyAndHough(cv::Mat& src, cv::Mat& dst, vector<Vec4i>& lines)
{
	//void Canny(InputArray image, OutputArray edges, double threshold1, double threshold2, int apertureSize=3, bool L2gradient=false )
	//
	Mat src_gray;
	cvtColor( src, src_gray, CV_BGR2GRAY );
	blur( src_gray, src_gray, Size(5,5) );
	Canny(src_gray, src_gray, cT1, cT1*3);
	//vector<Vec4i> lines;
	HoughLinesP( src_gray, lines, 1, CV_PI/180, 80, 30, 10 );
	for( size_t i = 0; i < lines.size(); i++ )
	    {
	        line(src, Point(lines[i][0], lines[i][1]),
	            Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
	    }
	//HoughLinesP(InputArray image, OutputArray lines, double rho, double theta, int threshold, double minLineLength=0, double maxLineGap=0 )
}

void poly(Mat& src, vector<Vec4i>& lines)
{
	//for ( auto it=lines.begin(); it!=lines.end( ); ++it)
		for ( int i=0; i<lines.size();i++)
		{
			Vec4i line = lines.at(i);
			Point polyPoints[4];

			// Fi r s t point i s to the l e f t o f the r i g h t point
			 if (line[0 ] <= line[2])
			 {
				 polyPoints[0]=Point(line[0],0);
				 polyPoints[1]=Point(line[2],0);
				 polyPoints[2]=Point(line[2],line[3]);
				 polyPoints[3]=Point(line[0],line[1]);
			 }
			 else // Fi r s t point i s to the r i g h t o f the r i g h t point
			 {
				 polyPoints[0]=Point(line[2],0);
				 polyPoints[1]=Point(line[0],0);
				 polyPoints[2]=Point(line[0],line[1]);
				 polyPoints[3]=Point(line[2],line[3]);
			 }

			 fillConvexPoly( src , polyPoints , 4 , Scalar (0, 0, 0));
		 }
}

int main (int argc, char *argv[])
{
	init();
	FloorFinder1* ff = new FloorFinder1(folder, ext, rectHeight, rectWidth);

	cv::namedWindow("original");
	cv::namedWindow("floor");
	cv::namedWindow("canny");

	cv::createTrackbar("height", "original", &p, 255, trackBarCbHeight);
	cv::createTrackbar("width", "original", &p, 255, trackBarCbWidth);

	cv::createTrackbar("threshold1", "canny", &cT1, cTmax, trackBarCT1);
	cv::createTrackbar("threshold2", "canny", &cT2, cTmax, trackBarCT2);

	cv::Mat out;
	cv::Mat src;
	cv::Mat out2;
	char c=0;
	char cl=0;

	vector<Vec4i> lines;

	if(ff->readImage())
	{
		while(c!=27)
		{
			src = ff->getImg();
			if(src.rows>0)
			{
				ff->rectHeight = rectHeight;
				ff->rectWidth = rectWidth;

				out2.create(src.size(), src.type());
				cannyAndHough(src, out2, lines);

				out = ff->process();
				out2 = out.clone();
				poly(out2, lines);

				cv::imshow("floor", out);
				cv::rectangle(src, cv::Rect(cv::Point((src.cols-ff->rectWidth-1)/2,src.rows-ff->rectHeight),
										cv::Point((src.cols+ff->rectWidth-1)/2, src.rows)), cv::Scalar(255, 0, 0));
				cv::imshow("original", src);
				cv::imshow("canny", out2);

				c = cv::waitKey(33);
			}

			if(c == cl) {}
			else if(c==' ')
			{
				DBG("Space -- Read next image...");
				bool f = ff->readImage();
				if(!f)
					break;
			}

			cl = c;
		}
	}
	else
		exit(-1);

	DBG("Processed "+boost::lexical_cast<std::string>(i)+" images");
	return 0;
}
