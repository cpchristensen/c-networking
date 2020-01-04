#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <time.h>

#include "networking.h"

#define CHUNK_SIZE 1024

int main () {
    client_t* client = connectToServer("192.168.1.162", 1234);

    int total_data_size = 1;
    char buffer[CHUNK_SIZE] = {0};
    int total_read = 0;

    clock_t timer_start;
    double elapsed;

    timer_start = clock();
    while (total_read < total_data_size) {
        total_read += recv(client->fd, buffer, CHUNK_SIZE, 0);
    }
    elapsed = (double) (clock() - timer_start) / (double) CLOCKS_PER_SEC;

    printf("Elapsed time: %.2lfs.\n", elapsed);
	puts(buffer);

    close(client->fd);
    free(client);

    return 0;
}
