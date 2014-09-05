#include "request.h"
#include "socket.h"

#include <iostream>
#include <sstream>

using namespace std;

namespace
{
    int GetProtocol(char* proto)
    {
        if(proto == "tcp")
            return SOCK_STREAM;
        else
            return -1;
    }
}

int main(int argc, char *argv[])
{

    if(argc<4)
    {
        cout<< "to few parameters"<<endl;
        return 1;
    }

    string host = argv[0];
    int port;
    istringstream(argv[1]) >> port;
    int proto = GetProtocol(argv[2]);
    string message = argv[3];
        
    const int try_count = 10;
    for(int i=0; i<try_count; ++i)
    {
        try
        {
            cout << Client::SendRequest(host, port, proto, message);
            break;
        } 
        catch (const char* s)
        {
            cerr << "Catch error:" << s << endl;
        } 
        catch (...)
        {
            cerr << "unhandled exception\n";
        }
        Sleep(1000);
    }
    return 0;
}
