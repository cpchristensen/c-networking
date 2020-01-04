#ifndef NETWORKING_H
#define NETWORKING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define MAX_CLIENTS_CONNECTED 1
#define FAILED_TO_ACCEPT 2

typedef struct {
	int fd, port, addrlen;

	int max_clients, connected_clients;
	int* clients;

	struct sockaddr_in address;
} server_t;

typedef struct {
	int fd;
	struct sockaddr_in server_addr;
} client_t;

int setBlocking (int fd, int is_blocking);
server_t* serverInit (int port, int max_clients, int backlog);
int acceptConnection (server_t* server);
void killServer (server_t* server);

client_t* connectToServer (const char* ip, int port);

#endif
