#pragma once

#include <WinSock2.h>
#include <string>
#include <memory>

class Socket
{
public:
  virtual ~Socket();
  Socket(const Socket&);
  Socket& operator=(Socket&);
  SOCKET Get();
  void Set(SOCKET s);    
    
  virtual std::string Receive() = 0;
  // The parameter of SendLine is not a const reference
  // because SendLine modifes the std::string passed.
  virtual void  Send(std::string) = 0;
    
protected:
  Socket(SOCKET s);
  Socket();
  SOCKET s_;
};

class SocketTCP : public Socket
{
public:
    SocketTCP();
    std::string Receive();
    void Send(std::string);
};
    
class SocketClient
{
public:
    SocketClient(const std::string& host, int port, int proto);
    std::string Receive();
    void  Send(std::string);
    
private:
    Socket* socket;
};


class SocketServer
{
public:
  SocketServer(int port, int proto);
  Socket* Accept();
  SOCKET Get();  
private:
    Socket* socket;
    int protocol;
    int max_connections;
    
    
};

class SocketSelect
{
  public:
    SocketSelect(SOCKET s);
    bool Readable(SOCKET s);

  private:
    fd_set fds_;
}; 

