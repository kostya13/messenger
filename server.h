#pragma once

#include <windows.h>
#include <iostream>
#include <fstream>


struct State
{
    State() : run(true), waitstop(true){};
    bool run;
    bool waitstop;
};

class FileWriter
{
public:
    FileWriter(const std::string& filename)
        {
            fs.open (filename.c_str(), std::fstream::out | std::fstream::app);               
        }

    ~FileWriter()
        {
            fs.close();
        }

    void Write(std::string data)
        {
            std::cout << data << std::endl;           
            fs << data << std::endl;
        }
private:
    std::fstream fs;
};

class Logger
{
public:
    Logger(const std::string& logfile) : writer(logfile)
        {
            mutex = CreateMutex(NULL, FALSE, NULL); 
            if (mutex == NULL) 
            {
                std:: cout << "CreateMutex error: %d\n" <<  GetLastError() << std::endl;
            }
        };
    
    void AddEntry(const std::string& data)
        {
            WaitForSingleObject(mutex, INFINITE);
            std::string tmp = data;
            ReleaseMutex(mutex);
            writer.Write(tmp);
        }
             
private:
    FileWriter writer;
    HANDLE mutex;
};

