A software system for receiving messages.

General information

This system consists of two components: server and client.
The server can listen on multiple ports.

Currently implemented only support TCP and UDP protocols.

The server setup is done via a configuration file in ini format for Windows.
An example file is included with the server.

The server also write all recieved messages in the logfile.

The client is presented in two versions: the console to send a single message (also useful for testing).
And with a graphical interface that allows you to send multiple messages
(if a server not responds, client makes several attempts to send the message).

All results of sent messages are added to the list, which is stored until the end of the program.

Server architecture.

For each listening port is created a separate thread.
Each recieved message written to the log file.
The logger runs in the main thread, all communication with the logger synchronized via a mutex.
The data to logger is transmitted via a single variable.
The queue is not used because at low loads, the performance difference is not noticeable, and the implementation of the buffer through one variable is much easier.

Protocol:
The message is simple string.
For each received message, the client responds
OK.

Features
All implementation details are not specified in the task was done as simple as possible way.
For exception handling is not used some special class hierarchy, and simply sends a string that specifies caught the error.

The server does not create a separate thread for the new accept connection because it will not increase performance.
In this implementation bottleneck is the transfer line to the logger(one logger maintains all connections).
In high load application more realistic logger must use a separate thread and pass the data through some thread-safe queue.
If some things in the programs can be done in different ways, I chose the one that is easier to implement.
I am don't write comment if name of function or variable is obvious or trivial.

I'm use mingw  c++ compiler to build programs.
