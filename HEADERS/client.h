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
#define CLIENT_SERVER_ADDRESS "127.0.0.1" // The server's pc address
#define CLIENT_MAX_BUFFER_SIZE 1024 // Max buffer size
#define CLIENT_MAX_NAME_SIZE 20 // Max acceptable name length
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

extern pthread_mutex_t mutex_print, mutex_socket_funcs;

/**
 *@brief struct to store a single client
 *
 */
typedef struct client_struct{
    char name[CLIENT_MAX_NAME_SIZE]; // CLient's name
    uint32_t *msg_array; // a dynamic array of indexes for all messages sent by this client
    uint32_t msg_array_len; // the length of msg_array
    uint32_t socket_fd; // this clients socket fd
    int32_t id; // the index of the client in the clients_array
} client_t;


/**
 *@brief This thread recieve input message from the client and send it to the server
 *
 *
 *@param send_msg_args -> client id and file descriptor
 *
 */
void* pthread_send_message(void* send_msg_args);

/**
 *@brief Handle and print recieved messaged for the client
 *
 *This thread recieve and print messages on the client screen
 *
 *@param sock_fd the client socket fd
 *
 *@return the clients index in clients array on sucsses, else -1
 */
void* pthread_recv_message(void* sock_fd);
#endif //CLIENT_H
