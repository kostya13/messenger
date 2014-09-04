#include <windows.h>

#include "server.h"

namespace Server
{
   struct ThreadData
   {
       ThreadData(int _proto, int _port, Logger* _l, State* _s)
           : proto(_proto), port(_port), logger(_l), state(_s){};
       int proto;
       int port;
       Logger* logger;
       State* state;
   };
  
   DWORD WINAPI ConnectionThread(void* threadParams);
}
