#include "log.h"
#include "FloorFinder1.h"

using namespace std;
using namespace cv;

int main (int argc, char *argv[])
{

	FloorFinder1* ff = new FloorFinder1("./conf.xml", 1);

	try
	{
		ff->loop();

	}
	catch(cv::Exception & e)
	{
		DBG(e.msg);
		DBG(e.err);
	}
	catch(int i)
	{
		DBG(i);
	}

	return 0;
}
