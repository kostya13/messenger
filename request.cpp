#include "request.h"
#include "socket.h"

namespace Client
{
    std::string  SendRequest(const std::string& host, int port, int proto, const std::string& message)
    {
            SocketClient s(host, port, proto);
            s.Send(message);
            return  s.Receive();
    }
}
