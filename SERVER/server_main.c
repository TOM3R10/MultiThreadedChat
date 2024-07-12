#include "../HEADERS/headers.h"


int main(void) {
// Create the server data
    server_t *server_data = (server_t *)malloc(sizeof(server_t));
    if (!server_data) exit(1);


    int server_socket_fd, client_socket_fd, addr_size;

    // Creating a mutex to lock and unlock the clients array
    if (pthread_mutex_init(&mutex_array_lock, NULL) != 0) {
        perror("Mutex creation has failed\n");
        return 1;
    }

    // Creating a mutex to lock and unlock the server_data
    if (pthread_mutex_init(&mutex_server_data_lock, NULL) != 0) {
        perror("Mutex creation has failed\n");
        return 1;
    }


    SA_IN server_addr, client_addr;

    if ((server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Failed to create a socket");
        return 1;
    }

    printf("[SERVER]Created socket...\n");
    sleep(1);
    fflush(stdout);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_SERVER_PORT);

    if (bind(server_socket_fd, (SA*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Faild to bind to a port");
        return 1;
    }

    printf("[SERVER]Socket connected to port %d....\n", SERVER_SERVER_PORT);
    sleep(1);
    fflush(stdout);

    if (listen(server_socket_fd, SERVER_LISTEN_TRIES) == -1) {
        perror("Listen falied");
        return 1;
    }

    printf("[SERVER]Waiting For A User To Connect....\n");
    fflush(stdout);

    // Accepting clients connections
    while(clients_connected <= SERVER_MAX_CLIENTS) {
        addr_size = sizeof(SA_IN);

        if ((client_socket_fd = accept(server_socket_fd,
            (SA*)&client_addr,
            &addr_size)) == -1) {
            perror("No connection was captured, listen falied");
            //return 1;
            }

        printf("[SERVER]Connection captured\n");
        fflush(stdout);


        // Handle a client connection
        pthread_t hanlde_connection_thread_id;

        // Init a new client struct
        client_t *client = (client_t *)malloc(sizeof(client_t));
        if (!client) {
            perror("Malloc failed ");
            continue;
        }

        bzero(client, sizeof(client_t));
        client->socket_fd = client_socket_fd;


        // adding the client to the array
        int client_id = add_client(server_data, client);

        // Sending the client thier id
        if (send(client->socket_fd, &client_id, sizeof(client_socket_fd), 0) < 0) {
            perror("Cant send the clients thier id\n");
            close(client->socket_fd);
        }

        *client = server_data->clients_arr[client_id];


        pthread_hc_args *args = (pthread_hc_args *)malloc(sizeof(pthread_hc_args));
        bzero(args, sizeof(pthread_hc_args));

        int *client_id_p = &client_id;

        args->client_id = client_id_p;
        args->server_pointer = server_data;

        if (pthread_create(&hanlde_connection_thread_id,
            NULL, pthread_handle_connection, args) != 0)
            {
            perror("Failed to create thread");
            free(client);
            close(client_socket_fd);
        }


        else {
            pthread_detach(hanlde_connection_thread_id);  // Detach the thread
        }

    }
    close(server_socket_fd);
    return 0;

}
