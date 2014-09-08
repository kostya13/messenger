#include "Socket.h"
#include <iostream>

using namespace std;

namespace
{
    const int MAXCONN = 10;
}   
 

Socket:: Socket() : s_(0)
{
}

Socket::Socket(SOCKET s) : s_(s)
{
}

Socket::~Socket()
{
  closesocket(s_);
}

Socket::Socket(const Socket& o)
{
  s_ = o.s_;
}

Socket& Socket::operator=(Socket& o)
{
  s_ = o.s_;
  return *this;
}

Socket& Socket::operator=(SOCKET s)
{
  s_ = s;
  return *this;
}

SOCKET Socket::Get()
{
    return s_;
}

SocketTCP::SocketTCP()
{
  s_ = socket(AF_INET,SOCK_STREAM,0);
  if (s_ == INVALID_SOCKET)
  {
    throw "INVALID_SOCKET";
  }
}

SocketTCP::SocketTCP(SOCKET s) : Socket(s)
{
};


std::string SocketTCP::Receive()
{
  std::string ret;
  while (1)
  {
    char r;
    switch(recv(s_, &r, 1, 0))
    {
      case 0:
        return ret;
      case -1:
        return "";
    }
    ret += r;
    if (r == '\n')  return ret;
  }
}

void SocketTCP::Send(std::string s)
{
  s += '\n';
  send(s_,s.c_str(),s.length(),0);
}

ServerTCP::ServerTCP(int port)
    :  max_connections(MAXCONN)
{
  sock = new SocketTCP();
  
  sockaddr_in sa;

  memset(&sa, 0, sizeof(sa));

  sa.sin_family = PF_INET;             
  sa.sin_port = htons(port);          
  
// Set non blocking socket
    u_long arg = 1;
    ioctlsocket(sock->Get(), FIONBIO, &arg);

  /* bind the socket to the internet address */
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

void ServerTCP::Send(std::string str)
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

std::string ClientTCP::Receive()
{
    return sock->Receive();
}

void ClientTCP::Send(std::string str)
{
    sock->Send(str);
}


SocketSelect::SocketSelect(SOCKET s)
{
  FD_ZERO(&fds_);
  FD_SET(s, &fds_);

  TIMEVAL tval;
  tval.tv_sec  = 0;
  tval.tv_usec = 1;

  TIMEVAL *ptval;
  ptval = &tval;
 
  if (select (0, &fds_, (fd_set*) 0, (fd_set*) 0, ptval) == SOCKET_ERROR)
  {
    throw "Error in select";
  }
}

bool SocketSelect::Readable(SOCKET s)
{
  if (FD_ISSET(s,&fds_))
  {
      return true;
  }
  return false;
}


SocketIO* CreateServer(int proto, int port)
{
    switch(proto)
    { 
    case SOCK_STREAM:
        return new ServerTCP(port);
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
    default:
        throw "Unknown client type";
    }
}
