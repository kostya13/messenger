#include "services.h"
#include <iostream>
using namespace std;
 
namespace
{
    const int MAXCONN = 10;
}   

std::string SocketIO::Receive()
{
    return sock->Receive();
}

void  SocketIO::Send(const std::string& str)
{
    sock->Send(str);
}


ServerTCP::ServerTCP(int port)
    : max_connections(MAXCONN)
{
  sock = new SocketTCP();
  
  sockaddr_in sa;
  memset(&sa, 0, sizeof(sa));

  sa.sin_family = PF_INET;             
  sa.sin_port = htons(port);          
  
 // Set non blocking socket
    u_long arg = 1;
    ioctlsocket(sock->Get(), FIONBIO, &arg);

  if (bind(sock->Get(), (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR)
  {
    closesocket(sock->Get());
    throw "INVALID_SOCKET";
  }
  
  listen(sock->Get(), max_connections);                               
}

SOCKET ServerTCP::Accept()
{
  SOCKET new_sock = accept(sock->Get(), 0, 0);
  if (new_sock == INVALID_SOCKET)
  {
      throw "Invalid socket in accept";
  }
  return new_sock;
}

std::string ServerTCP::Receive()
{
    string data;
    SocketSelect sel(sock->Get());            
    if (sel.Readable(sock->Get()))
    {
        accept_socket = new SocketTCP(Accept());
        data = accept_socket->Receive();
    }
    return data;
}

void ServerTCP::Send(const std::string& str)
{
    accept_socket->Send(str);
    delete accept_socket;
}

ClientTCP::ClientTCP(const std::string& host, int port)
{
  sock = new SocketTCP();
  
  std::string error;

  hostent *he;
  if ((he = gethostbyname(host.c_str())) == 0)
  {
    error = strerror(errno);
    throw error.c_str();
  }
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr = *((in_addr *)he->h_addr);
  memset(&(addr.sin_zero), 0, 8); 
  if (::connect(sock->Get(), (sockaddr *) &addr, sizeof(sockaddr)))
  {
    error = strerror(WSAGetLastError());
    throw error.c_str();
  }
}

ClientUDP::ClientUDP(const std::string& host, int port)
{
  std::string error;

  hostent *he;
  if ((he = gethostbyname(host.c_str())) == 0)
  {
    error = strerror(errno);
    throw error.c_str();
  }
  sockaddr_in addr;
  memset((char *) &addr, 0, sizeof(addr));  
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr = *((in_addr *)he->h_addr);
  sock = new SocketUDP(addr);  
}

ServerUDP::ServerUDP(int port)
{
  sock = new SocketUDP();  

  sockaddr_in sa;
  memset(&sa, 0, sizeof(sa));
  sa.sin_family = PF_INET;
  sa.sin_addr.s_addr = INADDR_ANY;  
  sa.sin_port = htons(port);          
  
 // Set non blocking socket
    u_long arg = 1;
    ioctlsocket(sock->Get(), FIONBIO, &arg);

  if (bind(sock->Get(), (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR)
  {
    closesocket(sock->Get());
    throw "INVALID_SOCKET";
  }
}

std::string ServerUDP::Receive()
{
    string data;
    SocketSelect sel(sock->Get());            
    if (sel.Readable(sock->Get()))
    {
        data = sock->Receive();
    }
    return data;
}


SocketIO* CreateServer(int proto, int port)
{
    switch(proto)
    { 
    case SOCK_STREAM:
        return new ServerTCP(port);
        break;
    case SOCK_DGRAM:
        return new ServerUDP(port);
        break;
    default:
        throw "Unknown server type";
    }
}

SocketIO* CreateClient(int proto, const std::string& host, int port)
{
    switch(proto)
    { 
    case SOCK_STREAM:
        return new ClientTCP(host, port);
        break;
    case SOCK_DGRAM:
        return new ClientUDP(host, port);
        break;
    default:
        throw "Unknown client type";
    }
}
