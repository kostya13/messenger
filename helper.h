#include "common.h"
#include <string>
#include <sstream>

// if string not valid number (for example "abc") function return 0;
inline int CharToInt(const char* str)
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

