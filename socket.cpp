#include "common.h"
#include "socket.h"

#include <iostream>

using namespace std;
 
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
    // max leight of client message is  BUF_SIZE
    char r[BUF_SIZE];
    memset(r, 0, BUF_SIZE);        
    switch(recv(s_, r, BUF_SIZE, 0))
    {
    case 0:
        return std::string();
    case SOCKET_ERROR:
        throw "Recv socket error";
    }
    std::string ret(r);    
    return ret;
}

void SocketTCP::Send(const std::string& s)
{
    
    if(send(s_, s.c_str(), s.length(), 0) == SOCKET_ERROR)
        throw  "send() failed";
}

SocketUDP::SocketUDP()
{
  s_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (s_ == INVALID_SOCKET)
  {
    throw "INVALID_SOCKET";
  }
}

SocketUDP::SocketUDP(sockaddr_in addr) : si_other(addr)
{
  s_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (s_ == INVALID_SOCKET)
  {
    throw "INVALID_SOCKET";
  }
};

std::string SocketUDP::Receive()
{
    char r[BUF_SIZE];
    memset(r, 0, BUF_SIZE);
    int slen_local = slen;
    switch(recvfrom(s_, r, BUF_SIZE, 0, (struct sockaddr *) &si_other, &slen_local))
    {
    case 0:
        return std::string();
    case SOCKET_ERROR:
        throw "Recv socket error";
    }
    std::string ret(r);    
    return ret;
}

void SocketUDP::Send(const std::string& s)
{
    if (sendto(s_, s.c_str(), s.size() , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
    {
        throw  "sendto() failed";
    }

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
  if (FD_ISSET(s, &fds_))
  {
    return true;
  }
  return false;
}
