/*
  Messenger server
 */

#include <windows.h>
#include <algorithm>
#include <list>
#include <iostream>
#include <fstream>

#include "connection.h"
#include "confreader.h"
#include "netsetup.h"
#include "server.h"

const std::string config_name  = ".\\server.ini";
const std::string logfile_name = "server.log";
const std::string config_section = "Ports";
const std::string key_tcp = "TCP";
const std::string key_udp = "UDP";

namespace
{
    State state;

    BOOL WINAPI ConsoleHandler(DWORD CEvent)
    {
        state.run = false;    
        while(state.waitstop){};
        return TRUE;
    }
    
   class ConnectionsPool
   {
   public:
       ConnectionsPool(Logger* logger, State* state)
       {
           std::list<int> tcp = IniFile::GetIntList(config_name, config_section, key_tcp);
           for(std::list<int>::const_iterator i = tcp.begin(); i != tcp.end(); ++i)
           {
               pool.push_back(new Server::ThreadData(SOCK_STREAM, (*i), logger, state));
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
               threads[c] =CreateThread(
                   NULL,                   /* default security attributes.   */
                   0,                      /* use default stack size.        */
                   Server::ConnectionThread,/* thread function name.          */
                   (void*)(*i),             /* argument to thread function.   */
                   0,                      /* use default creation flags.    */
                   &ThreadID);     /* returns the thread identifier. */
               if( threads[c] == NULL )
               {
                   std::cout<< "CreateThread error: %d\n" << GetLastError() << std::endl;
                   throw 2;
               }
               c++;
           }
       }

       void Wait()
       {
           WaitForMultipleObjects(pool.size(), threads, TRUE, INFINITE);
           std::cout << "Threads closed" << std::endl;    
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
        Logger logger(logfile_name);
        logger.AddEntry("start");
        ConnectionsPool cp(&logger, &state);
        cp.CreateThreads();
        cp.Wait();
        logger.AddEntry("finish");
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
