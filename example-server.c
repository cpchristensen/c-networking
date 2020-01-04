#include <stdio.h>
#include <string.h>
#include <math.h>

#include "networking.h"

int main () {
    server_t *server;
	char* buffer = malloc(15);
	buffer[14] = 0;
    server = serverInit(1234, 10, 10);

    acceptConnection(server);

    printf("Receiving data...\n");
    /* send(server->clients[0], "Hello", strlen("Hello"), 0);*/
	recv(server->clients[0], buffer, 14, 0);
    printf("Data received.\n");
	puts(buffer);

    killServer(server);

    return 0;
}
