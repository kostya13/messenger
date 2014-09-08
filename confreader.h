/* 
   Configuration file reader 
*/
#include <list>
#include <string>

namespace IniFile
{
/**
    Get list int for coma separated numbers in specific key of ini file

   @param filename name of ini file
   @param section section in ini file
   @param key key in section
   @return list of ints 
*/    
  std::list<int> GetIntList(const std::string &filename, const std::string &section, const std::string &key);
}
