#Make file to bild all programs
CPP  = g++.exe
RC = windres

SERVERBIN = server.exe
SERVEROBJ  = server.o socket.o thread.o confreader.o logger.o

CLIENTCLIBIN = client_cli.exe
CLIENTCLIOBJ  = client_cli.o socket.o request.o

CLIENTGUIBIN = client_gui.exe
CLIENTOBJGUI = client_gui.o gui.o socket.o request.o

all: server client gui

server: $(SERVEROBJ)
	$(CPP) $(SERVEROBJ) -lws2_32  -o $(SERVERBIN)

client: $(CLIENTCLIOBJ)
	$(CPP) $(CLIENTCLIOBJ) -lws2_32  -o $(CLIENTCLIBIN)

gui:  $(CLIENTOBJGUI)
	$(CPP) $(CLIENTOBJGUI) -o $(CLIENTGUIBIN) -s -lws2_32 -lcomctl32 -Wl,--subsystem,windows

gui.o: gui.rc resource.h
	${RC} -I. -i $< -o $@

%.o: %.c
	$(CPP)  $<
