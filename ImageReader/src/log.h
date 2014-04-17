#ifndef IMREADER_INFORMATION_MACROS
#define IMREADER_INFORMATION_MACROS

#define _DEBUG_

#ifdef _DEBUG_
#define DBG(var) \
  std::cout<< "DBG: "<<" [" << (var) << "]"<< std::endl
#else
#define DBG(var)
#endif

#endif
