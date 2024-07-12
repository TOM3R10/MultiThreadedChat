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

#include "client.h"

#define SERVER_SERVER_PORT 8383 // Port number for server pc
#define SERVER_LISTEN_TRIES 100
#define SERVER_MAX_CLIENTS 100 // Maximum amount of clients in server
#define SERVER_ADMIN_MODE_PASSWORD "--Admin 1234" // ..
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

extern pthread_mutex_t mutex_array_lock, mutex_server_data_lock;
extern int clients_connected; // Amount of users connected
extern int messages_sent; // ammount of messages sent


/**
 *@brief store variables for handle_connection() func
 *
 */
typedef struct pthread_handle_connection_args {
    void *client_id;
    void *server_pointer;
}pthread_hc_args;


/**
 *@brief struct to store a single message
 *
 */
typedef struct messsage_struct {
    uint32_t buffer_size; // size of the buffer
    uint32_t msg_id; // msg_index in msg_array
    uint32_t client_id; // client index in clients_array of the sender
    char buffer[CLIENT_MAX_BUFFER_SIZE]; // 1024
}msg_t;


/**
 *@brief struct to contain and manage the server data
 *
 */
typedef struct server_data {
    client_t *clients_arr; // a dynamic array to store all the clients
    msg_t *msg_array; // a dynamic array to store all the messages
}server_t;


/**
 *@brief struct to store a packed message
 *A packed message is a message sent by a client to other clients via broadcast()
 *A packed message contains only a message buffer and the name of the sending client
 *
 */
typedef struct message_pack {
    char buffer[CLIENT_MAX_BUFFER_SIZE]; // Message buffer containg the message
    char sender_name[CLIENT_MAX_NAME_SIZE]; // The sending client name
}msg_pack_t;


/**
 *@brief Handle the client struct and send its messages to other clients
 *This function recv a message from the client print it in server and broadcast it to other clients
 *
 *@param args -> pthread_hc_args (the client id,
 *                                a pointer to the server data)
 */
void* pthread_handle_connection(void* args);


/**
 *@brief Adds a client to clients_array and inc clients array
 *
 *This function malloc \ realloc clients array to save memory for the new client
 *
 *@param server_data a pointer to server data
 *@param client a pointer to the client
 *
 *@return the clients index in clients array on sucsses, else -1
 */
int add_client(server_t *server_data, client_t *client);


/**
 *@brief Adds a client to messages_array and inc messages array
 *
 *This function malloc \ realloc messages array to save memory for the new client
 *
 *@param server_data a pointer to server data
 *@param message a pointer to the message
 *
 *@return the message index in messages array on sucsses, else -1
 */
int add_message(server_t *server_data, msg_t *message);


/**
 *@brief Convert a msg_t  into msg_pack_t
 *Used for sending mesages to other clients
 *
 *@param server_data a pointer to server data
 *@param msg a pointer to the message
 *
 *@return a pointer to the new msg_pack_t
 */
msg_pack_t* msg_pack_to_string(server_t *server_data, msg_t *msg);

/**
 *@brief Sends a message to all connected clients
 *
 *
 *@param msg_pack a pointer to server data
 *@param clients_array a pointer to the clients_array
 *
 */
void broadcast_message(client_t *clients_array, msg_pack_t *msg_pack);


/**
 *@brief Convert a buffer into a msg type
 *
 *This function malloc space for a new mag_t and converting the buffer to this type
 *
 *@param buffer a pointer to a message buffer
 *
 *@return a pointer to a new msg_t
 */
msg_t* extract_msg(char* buffer);

#endif //SERVER_H
