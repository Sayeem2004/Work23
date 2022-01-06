#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

#ifndef NETWORKING_H

#define NETWORKING_H
#define BUFFER_SIZE 1000
#define PORT "9845"
#define IP "127.0.0.1"

void print_error(int err, char *msg);
int server_setup();
int server_handshake(int listen_socket);
void server_reset(int client_socket);
int client_handshake();

#endif
