#pragma once

#include <WinSock2.h>
#include <string>
#include <memory>

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
  // The parameter of SendLine is not a const reference
  // because SendLine modifes the std::string passed.
  virtual void  Send(std::string) = 0;
    
protected:
  Socket();
  SOCKET s_;
};

class SocketTCP : public Socket
{
public:
    SocketTCP();
    SocketTCP(SOCKET s);    
    std::string Receive();
    void Send(std::string);
};

class SocketSelect
{
  public:
    SocketSelect(SOCKET s);
    bool Readable(SOCKET s);
  private:
    fd_set fds_;
}; 

class SocketIO
{
public:
    virtual std::string Receive() = 0;
    virtual void  Send(std::string) = 0;
    virtual ~SocketIO(){delete sock;}

protected:
    Socket* sock;
};

class ClientTCP : public SocketIO
{
public:
    ClientTCP(const std::string& host, int port);
    std::string Receive();
    void Send(std::string);
};


class ServerTCP : public SocketIO
{
public:
  ServerTCP(int port);
  std::string Receive();
  void Send(std::string);
   
private:
    SOCKET Accept();
    Socket* accept_socket;
    int max_connections;
};

SocketIO* CreateServer(int proto, int port);
SocketIO* CreateClient(int proto, const std::string& host, int port);
