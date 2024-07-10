#include "../HEADERS/client.h"
#include "../HEADERS/server.h"

int main() {
    int client_socket_fd;
    struct sockaddr_in server_addr;
    char buffer[CLIENT_BUFFER_SIZE];

    if ((client_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Failed to create a socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // assuming the server is running on localhost

    if (connect(client_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Failed to connect");
        close(client_socket_fd);
        return 1;
    }

    printf("Connected to server at 127.0.0.1:%d\n", SERVER_SERVER_PORT);
    fflush(stdout);

    while (1) {
        printf("Send message: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Send message to server
        if (send(client_socket_fd, buffer, strlen(buffer), 0) == -1) {
            perror("Failed to send message");
            close(client_socket_fd);
            exit(1);
        }
    }

    close(client_socket_fd);
    return 0;
}
