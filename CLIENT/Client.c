#include "../HEADERS/headers.h"


/*void add_msg_to_client_array(client_t *client, int msg_index) {
    // realloc memory in the array for this index
    client->msg_array = (uint *)realloc(client->msg_array, sizeof(uint) * ++client->msg_array_len);
    if (!client->msg_array) {
        perror("Realloc failed in [add_msg_to_client_array]");
    }
    client->msg_array[client->msg_array_len - 1] = msg_index;
}*/
pthread_mutex_t mutex_print, mutex_socket_funcs;


void* pthread_recv_message(void* sock_fd) {
    // Create a msg struct to store the buffer
    msg_pack_t msg_pack;

    pthread_mutex_lock(&mutex_print);
    int socket_fd = *((int *)sock_fd);
    pthread_mutex_unlock(&mutex_print);


    while(recv(
    socket_fd,
    &msg_pack,
    sizeof(msg_pack_t), 0) > 0) {

        pthread_mutex_lock(&mutex_print);
        printf("[%s]: %s\n", msg_pack.sender_name, msg_pack.buffer);
        // printf("\r\033[k");
        fflush(stdout);
        pthread_mutex_unlock(&mutex_print);
        usleep(50000);


    }
    pthread_exit(NULL);
}


void* pthread_send_message(void* fd) {
    msg_t *msg = (msg_t *)malloc(sizeof(msg_t));
    if (!msg)
        return NULL;

    bzero(msg, sizeof(msg_t));

    pthread_mutex_lock(&mutex_print);
    int client_fd = *(int *)(fd);
    pthread_mutex_unlock(&mutex_print);

    while (1) {

        pthread_mutex_lock(&mutex_print);

        printf("Send message: ");
        fflush(stdout);
        pthread_mutex_unlock(&mutex_print);
        fgets(msg->buffer, sizeof(msg->buffer), stdin);
        fflush(stdin);

        // Send message to server
        if (send(client_fd, msg, sizeof(msg_t), 0) == -1) {
            perror("Failed to send message");
            close(client_fd);
            exit(1);
        }
        usleep(50000);
    }

    pthread_exit(NULL);
}
