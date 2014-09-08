#include <fstream>
#include <iostream>
#include <time.h>
#include <windows.h>

#pragma once
/**
   Global state of the server
 */
struct State
{
    State() : run(true), waitstop(true){};
    bool run;      // flag indicate: progrmam is running
    bool waitstop; // flag inidicate: whaiting all threads, while they closing
};

