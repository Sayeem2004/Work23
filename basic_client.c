#include "pipe_networking.h"

// Variable Declarations
char *buff;
int server_socket;

void client_exit() {
    // Closing Socket
    int err = close(server_socket);
    if (err == -1) {
        print_error(-1, "Client: Unable To Close Server Socket");
        return;
    } else printf("Client: Successfully Closed Server Socket\n");

    // Exiting Function
    free(buff);
    printf("Client: Successfully Shut Down\n");
    exit(0);
}

static void sighandler(int signo) {
    // SIGINT Case
    if (signo == SIGINT) {
        // Exiting Function
        printf("\n");
        client_exit();
    }

    // Exiting Function
    return;
}

int main() {
    // Signal Handling
    signal(SIGINT, sighandler);

    // Completing Handshake
    server_socket = client_handshake();
    if (server_socket == -1) return -1;

    // Initializing Buffer
    buff = calloc(BUFFER_SIZE, sizeof(char));

    // Constantly Runs
    while (1) {
        // Asking For User Input
        printf("Input String: ");
        fgets(buff, BUFFER_SIZE, stdin);
        *strchr(buff, '\n') = '\0';

        // Quit Or Exit Command
        if (strcmp(buff, "exit") == 0 || strcmp(buff, "quit") == 0) break;

        // Writing To User To Worker Pipe
        int err1 = write(server_socket, buff, BUFFER_SIZE);
        if (err1 == -1) {
            print_error(-1, "Unable To Send Input To Server");
            continue;
        }

        // Reading From Worker To User Pipe
        int err2 = read(server_socket, buff, BUFFER_SIZE);
        if (err2 == -1) {
            print_error(-1, "Unable To Read Output From Server");
            continue;
        }
        printf("Output String: %s\n", buff);
    }

    // Exiting Function
    client_exit();
}
