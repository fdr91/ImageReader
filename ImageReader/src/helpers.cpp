#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <tinyxml.h>
#include <stdexcept>

using namespace std;

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

string readConfigs(string xml, string root,string parameter)
{
	TiXmlDocument XMLdoc(xml);
	bool loadOkay = XMLdoc.LoadFile();
	if(!loadOkay)
	{
		throw runtime_error(XMLdoc.ErrorDesc());
		return "";
	}
	else
	{
		TiXmlElement *pRoot, *pParm, *pProcess, *pApp, *pLineFormat;
		pRoot = XMLdoc.FirstChildElement(root);
		if ( pRoot )
		{
			// Parse parameters
			pParm = pRoot->FirstChildElement("parameter");
			while ( pParm )
			{
				if(parameter.compare(pParm->Attribute("name"))==0)
						return pParm->Attribute("value");

				pParm = pParm->NextSiblingElement( "parameter" );
			}
		}
		else
		{
			return "";
		}
	}
	return "";
}
