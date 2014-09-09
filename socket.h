/*
  Wrappers for system sockets
  Interfaces for slient and server

  Now implements ony TCP sockets and server and clients, wiwh uses this socket
  Support for UDP sockets may be added with minimal effort.
 */
#include <memory>
#include <string>
#include <winsock2.h>

#pragma once

/*
   Base Socket class
 */
class Socket
{
public:
  virtual ~Socket();
  Socket(const Socket&);
  Socket(SOCKET s);
  Socket& operator=(Socket&);
  Socket& operator=(SOCKET);        
  SOCKET Get();

  virtual std::string Receive() = 0;
  virtual void  Send(const std::string& str) = 0;
    
protected:
  Socket();
  SOCKET s_;
  static const int BUF_SIZE = 256;    
};

/*
  Socket for TCP protocol 
*/
class SocketTCP : public Socket
{
public:
    SocketTCP();
    SocketTCP(SOCKET s);    
    std::string Receive();
    void Send(const std::string& str);
};

/*
  Socket for UDP protocol 
*/
class SocketUDP : public Socket
{
public:
    SocketUDP();
    SocketUDP(sockaddr_in addr);    
    std::string Receive();
    void Send(const std::string& str);
private:
    static const int slen = sizeof(sockaddr_in);
    sockaddr_in si_other;
};

/*
  Wrapper for system call select for non blocking sockets
*/
class SocketSelect
{
  public:
    SocketSelect(SOCKET s);
    bool Readable(SOCKET s);
  private:
    fd_set fds_;
}; 

