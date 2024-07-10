#ifndef SERVER_H
#define SERVER_H

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

#define SERVER_SERVER_PORT 8383
#define SERVER_LISTEN_TRIES 100
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

int start_server();

#endif //SERVER_H
