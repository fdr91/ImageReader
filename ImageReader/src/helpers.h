#ifndef IMAGE_READER_HELPERS
#define IMAGE_READER_HELPERS
#include <tinyxml.h>

int getdir (std::string dir, std::vector<std::string> &files);

std::string readConfigs(std::string xml, std::string root,std::string parameter);

#endif
