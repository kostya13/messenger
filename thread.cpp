/**
   Server thread function implementation
*/
#include "common.h"
#include "server.h"
#include "services.h"
#include "thread.h"

#include <iostream>
#include <memory>
#include <sstream> 

namespace
{
    std::string GetProtocolName(int protocol)
    {
        switch(protocol)
        {
        case SOCK_STREAM:
            return key_tcp;
            break;
        case SOCK_DGRAM:
            return key_udp;            
            break;
        default:
            throw "Unknown protocol type";
        }
    }
}

namespace Server
{
    DWORD WINAPI ConnectionThread(void* threadParams)
    {

        ThreadData* params = reinterpret_cast<Server::ThreadData*>(threadParams);

        std::ostringstream oss;
        oss << "Server listen on port: " << params->port << " protocol: " << GetProtocolName(params->proto);
        params->logger->AddEntry(oss.str());
        
        std::auto_ptr<SocketIO> server(CreateServer(params->proto, params->port));
        //state changes, when program prepare to close
        while(params->state->run)
        {
            std::string recieve = server->Receive();
            if (recieve != "")
            {
                params->logger->AddEntry(recieve);
                server->Send(Confirm);
            }
        }
        return 0;
    }
}
