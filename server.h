#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <time.h>

struct State
{
    State() : run(true), waitstop(true){};
    bool run;
    bool waitstop;
};

