#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#define CLIENT_SERVER_PORT 8383
#define CLIENT_SERVER_ADDRESS "127.0.0.1"
#define CLIENT_BUFFER_SIZE 1024
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

int start_client();

#endif //CLIENT_H
