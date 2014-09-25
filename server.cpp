/*
  Message server.

  This is simple multi thread server.
 */
#include "confreader.h"
#include "helper.h"
#include "logger.h"
#include "netsetup.h"
#include "server.h"
#include "thread.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <windows.h>

namespace
{
    static const std::string config_name  = ".\\server.ini";
    static const std::string logfile_name = "server.log";
    static const std::string config_section = "Ports";
    
    State state;

    //Handler to deal with pressed Ctrl+c, or close events
    BOOL WINAPI ConsoleHandler(DWORD CEvent)
    {
        state.run = false;
        //wait while all thread finished
        while(state.waitstop){};
        return TRUE;
    }


    // Pool of threads.
    // Server create one thread for each listen port
   class ConnectionsPool
   {
   public:
       ConnectionsPool(Server::Logger* logger, State* state)
       {
           typedef std::map<int,std::string> protocol_map;
           protocol_map protocols;
           protocols[SOCK_STREAM]=key_tcp;
           protocols[SOCK_DGRAM]=key_udp;

           for(protocol_map::const_iterator it = protocols.begin(), endit = protocols.end(); it != endit; ++it)
           {
               int proto = it->first;
               std::string key = it->second;
               std::list<int> added_ports;
               std::list<int> values = IniFile::GetIntList(config_name, config_section, key);
               for(std::list<int>::const_iterator i = values.begin(), endi = values.end(); i != endi; ++i)
               {
                   if(IsValidPortNumber((*i)))
                   {
                       std::list<int>::iterator findIter = std::find(added_ports.begin(), added_ports.end(), (*i));
                       if(findIter == added_ports.end())
                       {
                           pool.push_back(new Server::ThreadData(proto, (*i), logger, state));
                           added_ports.push_back((*i));
                       }
                   }
               }
           }
       }
       ~ConnectionsPool()
       {
           for(std::list<Server::ThreadData*>::const_iterator i = pool.begin(); i != pool.end(); ++i)
           {
               delete (*i);
           }
           delete[] threads;
       }
       
       void CreateThreads()
       {
           DWORD ThreadID;
           threads = new HANDLE[pool.size()];
           int c = 0;
           for(std::list<Server::ThreadData*>::const_iterator i = pool.begin(); i != pool.end(); ++i)
           {
               threads[c] = CreateThread(
                   NULL, 0, Server::ConnectionThread,
                   (void*)(*i), 0, &ThreadID);
               if( threads[c] == NULL )
               {
                   std::cout<< "CreateThread error: %d\n" << GetLastError() << std::endl;
                   throw "CreateThread error";
               }
               c++;
           }
       }

       void Wait()
       {
           WaitForMultipleObjects(pool.size(), threads, TRUE, INFINITE);
           std::cout << "All threads closed" << std::endl;    
       }
   
   private:
       std::list<Server::ThreadData*> pool;
       HANDLE* threads;       
   };
 
}
    
int main()
{
    if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE))
    {
        std::cout<< "\nERROR: Could not set control handler" << std::endl; 
        return 1;
    }

    try
    {
        NetSetup network;
        Server::Logger logger(logfile_name);
        logger.AddEntry("Server started");
        ConnectionsPool cp(&logger, &state);
        cp.CreateThreads();
        cp.Wait();
        logger.AddEntry("Server finished");
        state.waitstop = false;    
    }
    catch (const char* s) 
    {
        std::cout << "Error:" << s << std::endl;
    }
    catch(...)
    {
        std::cout << "Unknown error" << std::endl;        
    }
    return 0;
}
