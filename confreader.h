/* 
   Configuration file reader 
*/

#include <string>
#include <list>

namespace IniFile
{
  std:list<int> GetIntList(const std::string &filename, const std::string &section, const std::string &key);
}
