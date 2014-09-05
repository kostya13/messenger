#include <iostream>

#include "connection.h"
#include "server.h"
#include "socket.h"

namespace Server
{
    DWORD WINAPI ConnectionThread(void* threadParams)
    {

        ThreadData* params = (Server::ThreadData*)threadParams;
        std::cout << "Server listen on "<< params->proto << " " << params->port << std::endl;

        SocketServer in(params->port, 5, NonBlockingSocket);
        while(params->state->run)
        {
            SocketSelect sel(&in, NULL, NonBlockingSocket);            
            if (sel.Readable(&in))
            {
                Socket* s=in.Accept();
                std::string r = s->ReceiveLine();
//                std::cout << "Recived:" << r << std::endl;
//                if (r.empty()) continue;
                params->logger->AddEntry(r);
                s->SendLine("OK");
            }
        }
        return 0;
    }
}
