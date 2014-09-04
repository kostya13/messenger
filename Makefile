CPP  = g++.exe
OBJ  = server.o socket.o connection.o confreader.o
BIN = server.exe
BINCLIENT = client.exe
OBJCLIENT  = client.o socket.o

server: $(OBJ)
	$(CPP) $(OBJ) -lws2_32  -o $(BIN)

client: $(OBJCLIENT)
	$(CPP) $(OBJCLIENT) -lws2_32  -o $(BINCLIENT)

all: server client

%.o: %.c
	$(CPP)  $<
