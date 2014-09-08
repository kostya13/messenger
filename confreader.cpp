/*
  Implementation for config reader
*/
#include "confreader.h"
#include "helper.h"

#include <algorithm>
#include <iostream>
#include <windows.h>


namespace
{
    const int BUF_SIZE = 512;

    void IniRawRead(const char *filename, const char *section, const char *key, char* buffer)
    {
        const int FileNotFound = 2;
        int readed = GetPrivateProfileString(section, key, NULL, buffer, BUF_SIZE, filename);
        if(GetLastError() == FileNotFound)
            throw "Config file not found";
        if(readed < 1)
           throw "No data to read";
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

    std::list<int> StrToIntList(const char *section)
    {
        std::list<std::string> strings = SplitString(section, ',');
        std::list<int> ints;
        for(std::list<std::string>::const_iterator i = strings.begin(); i != strings.end(); ++i)
        {
            ints.push_back(CharToInt((*i).c_str()));
        }
        return ints;
    }
}

namespace IniFile
{
    std::list<int> GetIntList(const std::string &filename, const std::string &section, const std::string &key)
  {
      char raw_buffer[BUF_SIZE];
      IniRawRead(filename.c_str(), section.c_str(), key.c_str(), raw_buffer);
      std::list<int> intlist = StrToIntList(raw_buffer);
      return intlist;
  }
}

