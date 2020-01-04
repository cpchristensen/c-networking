#include "networking.h"

int setBlocking (int fd, int is_blocking) {
	int flags = fcntl(fd, F_GETFL, 0);

	if (flags == -1) {
		return -1;
	}

	flags = is_blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);

	return fcntl(fd, F_SETFL, flags);
}

server_t* serverInit (int port, int max_clients, int backlog) {
	server_t* server = (server_t*) malloc(sizeof(server_t));

	int server_fd, i;
	int one = 1;
	struct sockaddr_in address;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		fprintf(stderr, "Failed to create socket.\n");
		return NULL;
	}

	/* Allows address and port to be reused immediately after closing. */
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(int)) != 0) {
		fprintf(stderr, "Error when setting socket options.\n");
		return NULL;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);

	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) != 0) {
		fprintf(stderr, "Failed to bind to port.\n");
        perror("bind");
		return NULL;
	}

	if (listen(server_fd, backlog) != 0) {
		fprintf(stderr, "Error listening on socket.\n");
		return NULL;
	}

	server->fd = server_fd;

	server->address = address;
	server->addrlen = sizeof(address);
	server->port = port;

    if (max_clients < 1) {
        max_clients = 1;
    }
	server->max_clients = max_clients;
	server->clients = (int*) calloc(max_clients, sizeof(int));
    for (i = 0; i < max_clients; i++) {
        server->clients[i] = -1;
    }
	server->connected_clients = 0;

	return server;
}

int acceptConnection (server_t* server) {
    int i, tmp_fd;

    tmp_fd = accept(server->fd, (struct sockaddr *) &server->address, (socklen_t*) &server->addrlen);
	if (tmp_fd < 0) {
		fprintf(stderr, "Failed to accept new connection.\n");
		return FAILED_TO_ACCEPT;
	}

    if (server->connected_clients == server->max_clients) {
        return MAX_CLIENTS_CONNECTED;
    }

    for (i = 0; i < server->max_clients; i++) {
        if (server->clients[i] == -1) {
            server->clients[i] = tmp_fd;
            server->connected_clients++;
        }
    }

	return 0;
}

/* Note: close() returns an integer (zero on success). Need to check for that and implement error cases. */
void killServer (server_t* server) {
    int i;
    for (i = 0; i < server->max_clients; i++) {
        if (server->clients[i] != -1) {
            close(server->clients[i]);
        }
    }

	close(server->fd);

    free(server->clients);
	free(server);
}

client_t* connectToServer (const char* ip, int port) {
    client_t* client = (client_t*) malloc(sizeof(client_t));

    client->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->fd < 0) {
		printf("\n Socket creation error \n");
		return NULL;
	}

    memset(&client->server_addr, '0', sizeof(client->server_addr));
    client->server_addr.sin_family = AF_INET;
	client->server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &client->server_addr.sin_addr) != 1) {
		printf("\nInvalid address/ Address not supported \n");
		return NULL;
	}

    if (connect(client->fd, (struct sockaddr *) &client->server_addr, sizeof(client->server_addr)) != 0) {
		printf("\nConnection Failed \n");
		return NULL;
	}

    return client;
}
