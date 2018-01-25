forking: fserver client

fserver: forking_server.o pipe_networking.o subserver.o startup.o
	gcc -o server forking_server.o pipe_networking.o subserver.o startup.o

client: client.o pipe_networking.o startup.o
	gcc -o client client.o pipe_networking.o startup.o

client.o: client.c pipe_networking.h
	gcc -c client.c

forking_server.o: forking_server.c pipe_networking.h subserver.h
	gcc -c forking_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c pipe_networking.c

subserver.o: subserver.c
	gcc -c subserver.c

startup.o: startup.c startup.h
	gcc -c startup.c

clean:
	rm -rf *.o
	rm -rf *~
	rm -rf client
	rm -rf server