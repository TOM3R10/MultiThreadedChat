#include "../HEADERS/headers.h"

int main() {
    int bytes;
    client_t *client = (client_t *)malloc(sizeof(client_t));
    pthread_t send_thread, recv_thread;
;

    // Creating a mutex to lock the print
    if (pthread_mutex_init(&mutex_print, NULL) != 0) {
        perror("Mutex creation has failed\n");
        return 1;
    }

    // Creating a mutex to lock the socket functions
    if (pthread_mutex_init(&mutex_socket_funcs, NULL) != 0) {
        perror("Mutex creation has failed\n");
        return 1;
    }


    struct sockaddr_in server_addr;
    char buffer[CLIENT_MAX_BUFFER_SIZE];

    if ((client->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Failed to create a socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(CLIENT_SERVER_ADDRESS);  // Assuming the server is running on localhost

    if (connect(client->socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Failed to connect");
        close(client->socket_fd);
        return 1;
    }

    // Retreving the user id from the server
    if (recv(client->socket_fd, &client->id, sizeof(client->id), 0) == -1) {
        perror("Failed to receive client ID");
        close(client->socket_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at 127.0.0.1: %d You're id is %d\n", SERVER_SERVER_PORT, client->id);
    fflush(stdout);


    // define the user name for client
    printf("Choose your user name >> ");
    bzero(client->name, sizeof(client->name));
    fgets(client->name, CLIENT_MAX_NAME_SIZE, stdin);
    client->name[strcspn(client->name, "\n")] = 0;
    fflush(stdin);

    // Sending the user name to the server
    if (send(client->socket_fd, client->name, strlen(client->name), 0) == -1) {
        perror("Failed to send user name");
        close(client->socket_fd);
        free(client);
        exit(1);
    }


    if (pthread_create(&send_thread,
        NULL, pthread_send_message, (void *)&client->socket_fd) != 0)
    {
        perror("Failed to create thread");
        free(client);
        close(client->socket_fd);
    }
    else {
        pthread_detach(send_thread);  // Detach the thread
    }


    // Create a thread to recv messages from the server
    if (pthread_create(&recv_thread, NULL, pthread_recv_message, (void *)&client->socket_fd) != 0) {
        perror("Failed to create thread");
        close(client->socket_fd);
        free(client);
        return 1;
    }
    else {
        pthread_detach(recv_thread);  // Detach the thread
    }

    while(1) {
   pthread_join(send_thread, NULL);
   pthread_join(recv_thread, NULL);
}
    close(client->socket_fd);
    return 0;
}