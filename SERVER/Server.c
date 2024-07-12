#include "../HEADERS/headers.h"


pthread_mutex_t mutex_array_lock, mutex_server_data_lock;
int clients_connected = 0; // Amount of users connected
int messages_sent = 0; // ammount of messages sent


// Return client index on sucsees, and -1 if not
int add_client(server_t *server_data, client_t *client) {
    // Malloc if this is the first client
    if (clients_connected == 0) {
        server_data->clients_arr = (client_t *)malloc(sizeof(client_t));
        if (!server_data->clients_arr) {
            perror("Malloc failed at [add_client]");
            return -1;
        }
    }
    else {
        // Reallocing memory for the new user
        server_data->clients_arr = (client_t *)realloc(server_data->clients_arr, sizeof(client_t) * (clients_connected + 1));
        if (!server_data->clients_arr) {
            perror("Realloc failed at [add_client]");
            return -1;
        }
    }

    // Add the new client to the array
    client->id = clients_connected;
    server_data->clients_arr[clients_connected] = *client;
    clients_connected++;

    // return the index of the client in the clients_array
    return client->id;
}



int add_message(server_t *server_data, msg_t *message) {
    if (messages_sent == 0) {
        // Allocate memory for the first message
        server_data->msg_array = (msg_t *)malloc(sizeof(msg_t));
        if (!server_data->msg_array) {
            perror("Malloc failed at [add_message]");
            return -1;
        }
    }
    else {
        // Reallocate memory for additional messages
        msg_t *new_array = (msg_t *)realloc(server_data->msg_array, sizeof(msg_t) * (messages_sent + 1));
        if (!new_array) {
            perror("Realloc failed at [add_message]");
            return -1;
        }
        server_data->msg_array = new_array;
    }
    // Add the new message to the array
    server_data->msg_array[messages_sent] = *message;
    messages_sent++;

    // return the index of the message in the messages_sent array
    return messages_sent - 1;
}

msg_t* extract_msg(char* buffer) {
    if (!buffer) {
        return NULL;
    }

    msg_t *new_message = (msg_t *)malloc(sizeof(msg_t));
    if (!new_message) {
        perror("Malloc failed at [extract_msg]");
        return NULL;
    }

    bzero(new_message, sizeof(msg_t));

    // Copy the buffer data into the new message struct
    memcpy(new_message, buffer, sizeof(msg_t));
    return new_message;
}

msg_pack_t* msg_pack_to_string(server_t *server_data, msg_t *msg) {
    msg_pack_t *msg_pack = (msg_pack_t *)malloc(sizeof(msg_pack_t));
    bzero(msg_pack, sizeof(msg_pack_t));

    strncpy(msg_pack->buffer, msg->buffer, CLIENT_MAX_BUFFER_SIZE);
    printf("MESSAGE CLIENT ID IS: %d", msg->client_id);

    strncpy(msg_pack->sender_name, server_data->clients_arr[msg->client_id].name, CLIENT_MAX_NAME_SIZE);

    return msg_pack;
}


void broadcast_message(client_t *clients_array, msg_pack_t *msg_pack) {
    pthread_mutex_lock(&mutex_array_lock); // Lock the clients array

    for (int i = 0; i < clients_connected; i++) {
        int receiver_fd = clients_array[i].socket_fd;

            printf("%s\n", msg_pack->sender_name);

            // Check if the reciever is this client
            if (send(receiver_fd, msg_pack, sizeof(msg_pack_t), 0) == -1) {
                perror("Failed to send message");
                close(receiver_fd);
            }

    }

    pthread_mutex_unlock(&mutex_array_lock); // Unlock the clients array
}


void* pthread_handle_connection(void *args) {
    pthread_hc_args *arg = (pthread_hc_args *)args;
    client_t *client = (client_t *)malloc(sizeof(client_t));
    bzero(client, sizeof(client_t));

    // retrieve id;
    int *client_id = arg->client_id;

    pthread_mutex_lock(&mutex_server_data_lock);

    server_t *server_data = arg->server_pointer;

    client = &server_data->clients_arr[*client_id];
    pthread_mutex_unlock(&mutex_server_data_lock);

    // Retreieving the name of the client
    int name = 0;
    if (
        name = recv(client->socket_fd,
        client->name,
        CLIENT_MAX_NAME_SIZE, 0) < 0
    ) {
        perror("Cant recieve name from client");
        exit(1);
    }

    // Printing the clients name in server
    printf("(%d)[%s Joined the chat]\n", *client_id, client->name);


    // Recieving messages
    char buffer[CLIENT_MAX_BUFFER_SIZE];
    while(recv(client->socket_fd, buffer, sizeof(msg_t), 0) > 0) {


        // adding the message to the messages array
        msg_t *new_message = (msg_t *)malloc(sizeof(msg_t));
        bzero(new_message, sizeof(msg_t));


        new_message = extract_msg(buffer);

        // Adding the client id to the message
        new_message->client_id = client->id;

        // Locking the server data

        // Pack the message before sending
        pthread_mutex_lock(&mutex_server_data_lock);
        new_message->msg_id = add_message(server_data, new_message);
        msg_pack_t *packed_new_massaged = msg_pack_to_string(server_data, new_message);

        // printf("[SERVER ]Msg id %d\n", new_message->msg_id);

        if (new_message->msg_id != -1) {
            printf("[%s]: %s", packed_new_massaged->sender_name, packed_new_massaged->buffer);
            fflush(stdin);
        }

        // Broadcast message to all clients

        broadcast_message(server_data->clients_arr, packed_new_massaged);
        pthread_mutex_unlock(&mutex_server_data_lock);

        bzero(buffer, CLIENT_MAX_BUFFER_SIZE);
        free(packed_new_massaged);
        free(new_message);
    }
    close(client->socket_fd);
    return NULL;
}


// Later on....
/*void admin_menu(server_t *server_data) {
    return;

}

// Prints the names of all the clients
void print_clients_names(server_t *server_data) {
    for (int i = 0; i < clients_connected; i ++) {
        printf("%s\n", server_data->clients_arr[i].name);
    }
}*/