forking: fserver client

fserver: forking_server.o pipe_networking.o subserver.o startup.o
	gcc -o server forking_server.o pipe_networking.o subserver.o startup.o

client: client.o pipe_networking.o startup.o execute.o input.o
	gcc -o client client.o pipe_networking.o startup.o execute.o

client.o: client.c pipe_networking.h input.o
	gcc -c client.c

forking_server.o: forking_server.c pipe_networking.h subserver.h
	gcc -c forking_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c pipe_networking.c

subserver.o: subserver.c
	gcc -c subserver.c

startup.o: startup.c startup.h
	gcc -c startup.c

execute.o: execute.c execute.h input.h pipe_networking.h
	gcc -c execute.c

input.o: input.c input.h
	gcc -c input.c

clean:
	rm -rf *.o
	rm -rf *~
	rm -rf client
	rm -rf server
	rm -rf pubwkp

bleach:
	rm -rf users
	rm -rf *.o
	rm -rf *~
	rm -rf client
	rm -rf server