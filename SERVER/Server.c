#include "../HEADERS/server.h"
#include "../HEADERS/client.h"

void* handle_connection(void* client_sock_fd_pointer) {
    int client_sock_fd = *((int*)client_sock_fd_pointer);
    free(client_sock_fd_pointer);

    char buffer[CLIENT_BUFFER_SIZE];
    int amount_of_bytes_read;

    while((amount_of_bytes_read = read(
        client_sock_fd,
        buffer,
        CLIENT_BUFFER_SIZE)) > 0) {

        buffer[amount_of_bytes_read] = '\0';
        printf("Message recieved: %s", buffer);
        fflush(stdout);

    }

    close(client_sock_fd);
    return NULL;
}

int main() {
    int server_socket_fd, client_socket_fd, addr_size;

    SA_IN server_addr, client_addr;

    if ((server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Failed to create a socket");
        return 1;
    }

    printf("Created socket...\n");
    fflush(stdout);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_SERVER_PORT);

    if (bind(server_socket_fd, (SA*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Faild to bind to a port");
        return 1;
    }

    printf("Socket connected to port....\n");
    fflush(stdout);

    if (listen(server_socket_fd, SERVER_LISTEN_TRIES) == -1) {
        perror("Listen falied");
        return 1;
    }

    printf("Waiting....\n");
    fflush(stdout);

    while(true) {
        addr_size = sizeof(SA_IN);

        if ((client_socket_fd = accept(server_socket_fd,
            (SA*)&client_addr,
            &addr_size)) == -1) {
            perror("No connection was captured, listen falied");
            //return 1;
            }

        printf("Connection captured\n");
        fflush(stdout);

        pthread_t thread_id;
        int* pointer_to_client = malloc(sizeof(int));
        if (!pointer_to_client) {
            perror("Failed to allocate memory");
            close(client_socket_fd);
            continue;
        }

        *pointer_to_client = client_socket_fd;

        if (pthread_create(&thread_id, NULL, handle_connection, (void*)pointer_to_client) != 0) {
            perror("Failed to create thread");
            free(pointer_to_client);
            close(client_socket_fd);
        } else {
            pthread_detach(thread_id);  // Detach the thread to handle cleanup automatically
        }


    }
    close(server_socket_fd);
    return 0;
}