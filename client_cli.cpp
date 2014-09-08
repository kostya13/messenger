#include "helper.h"
#include "netsetup.h"
#include "request.h"
#include "socket.h"

#include <iostream>


using namespace std;

int main(int argc, char *argv[])
{

    if(argc<4)
    {
        cout<< "Too few parameters"<<endl;
        cout<< "usage: client_cli host port message"<<endl;
        return 1;
    }

    string host = argv[1];
    int port = CharToInt(argv[2]);
    if(!IsValidPortNumber(port))
    {
        cout<<"Icorrect port number"<<endl;
        return 2;        
    }
    string message = argv[3];

    cout<< host << " "<<port<<" "<<message<<endl;
    NetSetup network;
    try
    {
        string result;
        result = Client::SendRequest(host, port, SOCK_STREAM, message);
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
