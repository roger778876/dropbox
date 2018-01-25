forking: client fserver

basic: client bserver

fserver: server.o pipe_networking.o
	gcc -o server server.o pipe_networking.o

client: client.o pipe_networking.o
	gcc -o client client.o pipe_networking.o

bserver: basic_server.o pipe_networking.o
	gcc -o server basic_server.o pipe_networking.o

client.o: client.c pipe_networking.h
	gcc -c client.c

basic_server.o: basic_server.c pipe_networking.h
	gcc -c basic_server.c

server.o: server.c pipe_networking.h
	gcc -c server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c pipe_networking.c

clean:
	rm -rf server client
	rm -rf *.o
	rm -rf *~
	rm -rf a.out