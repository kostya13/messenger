CPP  = g++.exe
OBJ  = server.o
LINKOBJ = server.o
BIN = server.exe

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o server.exe

server.o: server.cpp
	$(CPP) -c server.cpp -o server.o
