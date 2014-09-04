CPP  = g++.exe
OBJ  = server.o socket.o connection.o confreader.o
BIN = server.exe

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -lws2_32  -o server.exe

%.o: %.c
	$(CPP)  $<
