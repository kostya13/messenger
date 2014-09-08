/*
  Thread specific declaration
*/
#include "logger.h"
#include "server.h"

#include <windows.h>

namespace Server
{
    /**
       Data, witch get every thread
     */
   struct ThreadData
   {
       ThreadData(int _proto, int _port, Server::Logger* _l, State* _s)
           : proto(_proto), port(_port), logger(_l), state(_s){};
       int proto;  // working protocol
       int port;   //listening port 
       Logger* logger; // logger to write messages
       State* state;   //global state of server
   };

   // Function for each thread 
   DWORD WINAPI ConnectionThread(void* threadParams);
}
