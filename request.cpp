#include "helper.h"
#include "request.h"
#include "socket.h"

namespace Client
{
    std::string  SendRequest(const std::string& host, int port, int proto, const std::string& message)
    {
        SocketIO* client = CreateClient(proto, host, port);
        client->Send(message);
        std::string data = client->Receive();
        delete client;
        return data;
    }

    std::string DescribeReply(const std::string& reply)
    {
        std::string result;
        if(CheckServerReply(reply))
            result = "Message sended";
        else
            result = "Message rejected";
        return result;
    }
    
}
