#include "Socket.h"
#include <iostream>

using namespace std;

namespace
{
    const int MAXCONN = 10;
    
    Socket* CreateSocket(int proto)
    {
        switch(proto)
        { 
        case SOCK_STREAM:
            return new SocketTCP();
            break;
        case SOCK_DGRAM:
            throw "Unimplemented UDP socket type";
            break;
//        return new SocketUDP(host, port);        
        default:
            throw "Unknown socket type";
        }
    }
}

Socket::Socket() : s_(0)
{
};


Socket::Socket(SOCKET s) : s_(s)
{
};

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

SOCKET Socket::Get()
{
    return s_;
}

void Socket::Set(SOCKET s)
{
    s_ = s;
}


SocketTCP::SocketTCP()
{
  s_ = socket(AF_INET,SOCK_STREAM,0);
  if (s_ == INVALID_SOCKET)
  {
    throw "INVALID_SOCKET";
  }
}

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

SocketServer::SocketServer(int port, int proto)
    : socket(CreateSocket(proto))
    , protocol(proto)
    , max_connections(MAXCONN)
{
  sockaddr_in sa;

  memset(&sa, 0, sizeof(sa));

  sa.sin_family = PF_INET;             
  sa.sin_port = htons(port);          
  
// Set non blocking socket
    u_long arg = 1;
    ioctlsocket(socket->Get(), FIONBIO, &arg);

  /* bind the socket to the internet address */
    if (bind(socket->Get(), (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR)
  {
      closesocket(socket->Get());
    throw "INVALID_SOCKET";
  }
  
  listen(socket->Get(), max_connections);                               
}

Socket* SocketServer::Accept()
{
  SOCKET new_sock = accept(socket->Get(), 0, 0);
  if (new_sock == INVALID_SOCKET)
  {
    int rc = WSAGetLastError();
    if(rc==WSAEWOULDBLOCK)
    {
      return 0; // non-blocking call, no request pending
    }
    else
    {
      throw "Invalid Socket";
    }
  }

  Socket* r = CreateSocket(protocol);
  r->Set(new_sock);
  return r;
}

SOCKET SocketServer::Get()
{
    return socket->Get();
}

SocketClient::SocketClient(const std::string& host, int port, int proto)
    : socket(CreateSocket(proto))
{
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

  if (::connect(socket->Get(), (sockaddr *) &addr, sizeof(sockaddr)))
  {
    error = strerror(WSAGetLastError());
    throw error.c_str();
  }
}

std::string SocketClient::Receive()
{
    return socket->Receive();
}

void SocketClient::Send(std::string str)
{
    socket->Send(str);
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
