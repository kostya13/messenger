/*
  Class for logging server activity
 */
#include <memory>
#include <string>
#include <windows.h>

#pragma once

namespace Server
{
    /**
       Interface fo classes which write data to disks or DB
     */
    class Writer
    {
    public:
        virtual void Write(std::string data) = 0;
        virtual ~Writer(){};
    };

    /**
       Logger use to save server actions
     */
    class Logger
    {
    public:
        Logger(const std::string& logfile);
        void AddEntry(const std::string& data);
             
    private:
        std::auto_ptr<Writer> writer;
        HANDLE mutex;
    };
}
        
