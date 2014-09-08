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

/*
  Base class for client and server
 */
class SocketIO
{
public:
    virtual std::string Receive() = 0;
    virtual void  Send(const std::string& str) = 0;
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
    std::string Receive();
    void Send(const std::string& str);
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

// Fabrics for create instance of server and client
SocketIO* CreateServer(int proto, int port);
SocketIO* CreateClient(int proto, const std::string& host, int port);
