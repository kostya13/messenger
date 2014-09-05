#include "socket.h"

#include <iostream>

using namespace std;

int main() {

    bool go = true;  
    while(go){    
        try {
            SocketClient s("localhost", 4242);
            s.SendLine("Message");
            string l = s.ReceiveLine();
            cout << l <<"!"<<endl;
            cout.flush();
            go = false;
        } 
        catch (const char* s) {
            cerr << s << endl;
        } 
        catch (std::string s) {
            cout << "ERROR:" << s<<endl;      
            cerr << s << endl;
        } 
        catch (...) {
            cerr << "unhandled exception\n";
        }
    }

    return 0;
}
