#ifndef HELPERS_H
#define HELPERS_H
#include <sstream>

// Thanks stackoverflow https://stackoverflow.com/questions/5590381/easiest-way-to-convert-int-to-string-in-c
template <typename T>
  std::string NumberToString ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }

#endif