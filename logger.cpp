/*
  Class for logging server activity
 */

#include "logger.h"

#include <fstream>
#include <iostream>
#include <time.h>

namespace
{
    /**
       Implementation Server::Writer
       This writer write messages to simple log file
     */
    class  FileWriter : public Server::Writer
    {
    public:
        FileWriter(const std::string& filename);
        ~FileWriter();
        void Write(std::string data);
    private:
        std::fstream fs;
        
    };


    FileWriter::FileWriter(const std::string& filename)
    {
        fs.open (filename.c_str(), std::fstream::out | std::fstream::app);               
    }

    FileWriter::~FileWriter()
    {
        fs.close();
    }

    void FileWriter::Write(std::string data)
    {
        static const int BUF_SIZE = 64;
        char buffer [BUF_SIZE];
        time_t t;
        struct tm * timeinfo;

        time(&t);
        timeinfo = localtime (&t);
            
        strftime (buffer, BUF_SIZE, "%Y-%m-%d %H:%M:%S:", timeinfo);
        std::cout  << data << std::endl;
        fs << buffer << data << std::endl;
    }
}

namespace Server
{

    Logger::Logger(const std::string& logfile) : writer(new FileWriter(logfile))
    {
        mutex = CreateMutex(NULL, FALSE, NULL); 
        if (mutex == NULL) 
        {
            std:: cout << "CreateMutex error: %d\n" <<  GetLastError() << std::endl;
            throw "CreateMutex error";
        }
    }
    
    void Logger::AddEntry(const std::string& data)
    {
        //This is bottle neck of multithread server.
        //To increase performance need implement thread safe queue.
        WaitForSingleObject(mutex, INFINITE);
        writer->Write(data);
        ReleaseMutex(mutex);        
    }
}
