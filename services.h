#include "socket.h"

#include <memory>
#include <string>
#include <winsock2.h>

#pragma once
/*
  Base class for client and server
 */
class SocketIO
{
public:
    virtual std::string Receive();
    virtual void  Send(const std::string& str);
    virtual ~SocketIO(){delete sock;}
protected:
    Socket* sock;
};

/*
  Client for TCP socket
 */
class ClientTCP : public SocketIO
{
public:
    ClientTCP(const std::string& host, int port);
};

/*
  Client for UDP socket
 */
class ClientUDP : public SocketIO
{
public:
    ClientUDP(const std::string& host, int port);
};

/*
  Server for TCP socket
*/
class ServerTCP : public SocketIO
{
public:
  ServerTCP(int port);
  std::string Receive();
  void Send(const std::string& str);
   
private:
    SOCKET Accept();
    Socket* accept_socket;
    int max_connections;
};

/*
  Server for UDP socket
*/
class ServerUDP : public SocketIO
{
public:
  ServerUDP(int port);
  std::string Receive();
};


// Fabrics for create instance of server and client
SocketIO* CreateServer(int proto, int port);
SocketIO* CreateClient(int proto, const std::string& host, int port);
