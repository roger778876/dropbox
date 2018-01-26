#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <signal.h>

#define RED_TEXT "\x1B[31m"
#define GREEN_TEXT "\x1b[32m"
#define COLOR_RESET "\x1b[0m"


#ifndef NETWORKING_H
#define NETWORKING_H

#define ACK "HOLA"
#define HANDSHAKE_BUFFER_SIZE 10
#define BUFFER_SIZE 1000
#define FILE_SIZE 100000
#define PREVIEW_SIZE 30

int server_handshake(int *to_client);
int client_handshake(int *to_server);

int server_setup();
int server_connect(int from_client);

#endif