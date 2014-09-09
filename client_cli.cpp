/**
   Simple command line client
 */
#include "helper.h"
#include "netsetup.h"
#include "request.h"
#include "socket.h"

#include <iostream>


using namespace std;

namespace
{
    static const int UNKNOWN = -1;
    int GetProtocol(const string& str)
    {
        if(str == "tcp")
        {
            return SOCK_STREAM;
        }
        else if (str == "udp")
        {
            return SOCK_DGRAM;
        }
        else
        {
            return UNKNOWN;
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc<5)
    {
        cout<< "Too few parameters"<<endl;
        cout<< "usage:\nclient_cli host port protocol message"<<endl;
        cout<< "protocol is: tcp|udp"<<endl;
        return 1;
    }

    string host = argv[1];
    int port = CharsToInt(argv[2]);
    if(!IsValidPortNumber(port))
    {
        cout<<"Icorrect port number"<<endl;
        return 2;        
    }

    int protocol = GetProtocol(argv[3]);
    if(protocol == UNKNOWN)
    {
        cout<<"Unknown protocol."<<endl;
        return 3;
    }
    
    string message = argv[4];

    NetSetup network;
    
    try
    {
        string result;
        result = Client::SendRequest(host, port, protocol, message);
        cout << Client::DescribeReply(result) << endl;
    } 
    catch (const char* s)
    {
        cerr << "Catch error:" << s << endl;
    } 
    catch (...)
    {
        cerr << "unhandled exception\n";
    }
    return 0;
}
