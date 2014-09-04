#include "socket.h"

#include <iostream>

using namespace std;

int main() {

  try {
    SocketClient s("localhost", 4242);

    s.SendLine("Message");

//    while (1) {
    {
      string l = s.ReceiveLine();
//      if (l == "OK") break;
      cout << l;
      cout.flush();
    }

  } 
  catch (const char* s) {
    cerr << s << endl;
  } 
  catch (std::string s) {
    cerr << s << endl;
  } 
  catch (...) {
    cerr << "unhandled exception\n";
  }

  return 0;
}
