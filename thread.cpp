/**
   Server thread function implementation
*/
#include "common.h"
#include "server.h"
#include "socket.h"
#include "thread.h"

#include <iostream>
#include <memory>

namespace Server
{
    DWORD WINAPI ConnectionThread(void* threadParams)
    {

        ThreadData* params = reinterpret_cast<Server::ThreadData*>(threadParams);
        std::cout << "Server listen on " << params->port << std::endl;

        std::auto_ptr<SocketIO> server(CreateServer(params->proto, params->port));
        while(params->state->run)
        {
            std::string recieve = server->Receive();
            if (recieve != "")
            {
                params->logger->AddEntry(recieve);
                server->Send(Confirm);
            }
        }
//        delete server;
        return 0;
    }
}
