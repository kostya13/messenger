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

        SocketServer in(params->port, params->proto);
        while(params->state->run)
        {
            SocketSelect sel(in.Get());            
            if (sel.Readable(in.Get()))
            {
                Socket* s = in.Accept();
                std::string r = s->Receive();
//                std::cout << "Recived:" << r << std::endl;
//                if (r.empty()) continue;
                params->logger->AddEntry(r);
                s->Send("OK");
                delete s;
            }
        }
        return 0;
    }
}
