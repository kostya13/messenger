/*
  Common small functions for common use
 */
#include "common.h"

#include <sstream>
#include <string>

// If string not valid number (for example "abc") function return 0;
inline int CharsToInt(const char* str)
{
    int result;
    std::istringstream(str) >> result;
    return result;
}


inline bool IsValidPortNumber(int num)
{
    return (num > 0) &&  (num <= 65535);
}

inline bool CheckServerReply(const std::string& result)
{
    return result == Confirm;
}

