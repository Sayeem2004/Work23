#include "pipe_networking.h"

// Variable Declarations
char *buff;
int client_socket, listen_socket, frk;

void server_exit() {
    if (frk == 0) { // Child
        // Closing Socket
        int err = close(client_socket);
        if (err == -1) {
            print_error(-1, "Sub-Server: Unable To Close Client Socket");
            free(buff);
            exit(-1);
        } else printf("Sub-Server: Successfully Closed Client Socket\n");

        // Exiting Function
        free(buff);
        printf("Sub-Server: Successfully Shut Down\n");
        exit(0);
    } else { // Parent
        // Closing Socket
        int err = close(listen_socket);
        if (err == -1) {
            print_error(-1, "Server: Unable To Close Client Socket");
            free(buff);
            exit(-1);
        } else printf("Server: Successfully Closed Client Socket\n");

        // Exiting Function
        free(buff);
        printf("Server: Successfully Shut Down\n");
        exit(0);
    }
}

static void sighandler(int signo) {
    // SIGINT Case
    if (signo == SIGINT) {
        // Exiting Normally
        server_exit();
    }

    // Exiting Function
    return;
}

int main() {
    // Signal Handling
    signal(SIGINT, sighandler);

    // Initializing Buffer
    buff = calloc(BUFFER_SIZE, sizeof(char));

    // Initializing Server Socket
    listen_socket = server_setup();
    if (listen_socket == -1) {
        return -1;
    }

    // Persistent Process
    while (1) {
        // Starting Handshake
        client_socket = server_handshake(listen_socket);
        if (client_socket == -1) {
            server_reset(client_socket);
            continue;
        }

        // Forking Server
        frk = fork();

        if (frk == 0) { // Child Process
            // Constantly Runs
            while (1) {
                // Reading From User To Worker Pipe
                int err1 = read(client_socket, buff, BUFFER_SIZE);
                if (err1 == -1) {
                    print_error(-1, "Unable To Read Input From Client");
                    continue;
                }
                if (err1 == 0) break;

                // String Manipulation (Upper Case)
                int i;
                for (i = 0; i < BUFFER_SIZE; i++) {
                    if (buff[i] == '\0') break;
                    buff[i] = toupper(buff[i]);
                }

                // Writing To Worker To User Pipe
                int err2 = write(client_socket, buff, BUFFER_SIZE);
                if (err2 == -1) {
                    print_error(-1, "Unable To Write Output To Client");
                    continue;
                }
            }

            // Exiting Child Process
            server_exit();
        } else if (frk ==  -1) { // Error
            print_error(-1, "Server: Unable To Fork Sub-Process");
        } else { // Parent Process
            // Resetting Handshake
            server_reset(client_socket);
        }
    }

    // Exiting Normally
    server_exit();
}
