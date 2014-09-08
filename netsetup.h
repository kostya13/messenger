/*
  Prepare Windows network subsystem
 */
#pragma once

#include <WinSock2.h>

class NetSetup
{
public:
    NetSetup()
    {
        WSADATA info;
        if (WSAStartup(MAKEWORD(2,0), &info))
        {
            throw "Could not start WSA";
        }
    }
    ~NetSetup()
    {
        WSACleanup();        
    }
};
