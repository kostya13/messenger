/*
  Implementation if config reader
*/
#include <windows.h>
#include <algorithm>
#include <list>
#include <iostream>
#include <sstream>
#include "confreader.h"

namespace
{
    const int buf_size = 512;
    char *IniRawRead(const char *filename, const char *section, const char *key)
    {
        char *out = new char[buf_size];
        GetPrivateProfileString(section, key, NULL, out, buf_size, filename);
        return out;
    }

    std::list<std::string> SplitString(const std::string& s, char seperator)
    {
        std::list<std::string> output;
        std::string::size_type prev_pos = 0, pos = 0;
        while((pos = s.find(seperator, pos)) != std::string::npos)
        {
            std::string substring( s.substr(prev_pos, pos-prev_pos) );
            output.push_back(substring);
            prev_pos = ++pos;
        }
        output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
        return output;
    }

    int StrToInt(const std::string& str)
    {
        int numb;
        std::istringstream(str) >> numb;
        return numb;
    }
    
    std:list<int> StrToIntList(const char *section)
    {
        std::list<std::string> strings = SplitString(section, ',');
        std::list<int> ints;
        for std::list::const_iterator i = strings.begin(); i < strings.end(); i++)
        {
            ints.push_back(StrToInt(*i));
        }
        return ints;
    }
}

namespace IniFile
{
  std:list<int> GetIntList(const std::string &filename, const std::string &section, const std::string &key)
  {
      char *raw_buffer = new char[buf_size];
      raw_buffer = IniRawRead(filename.c_str(), section.c_str(), key.c_str());
      std:list<int> intlist = StrToIntList(raw_buffer);
      return intlist;
  }
}

