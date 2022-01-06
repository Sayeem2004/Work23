#include "pipe_networking.h"

void print_error(int err, char *msg) {
    if (err == -1) {
        // If Error Print Error Message and Errno
        printf("Error: %s\n", msg);
        printf("%s\n", strerror(errno));
    }
}

int server_setup() {
    // Setting Up Hints Struct
    struct addrinfo *hints;
    hints = calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_INET; // IPv4
    hints->ai_socktype = SOCK_STREAM; // TCP Socket
    hints->ai_flags = AI_PASSIVE; // Only Needed On Server

    // Getting IP Information
    struct addrinfo *results;
    int err1 = getaddrinfo(NULL, "9845", hints, &results);  // Server Sets Node To NULL
    if (err1 == -1) {
        print_error(-1, "Server: Unable To Get IP Information");
        return -1;
    } else printf("Server: Successfully Got IP Information");

    // Create Socket
    int listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    if (listen_socket == -1) {
        print_error(-1, "Server: Unable To Create Socket");
        return -1;
    } else printf("Server: Successfully Created Socket");

    // Binding Socket
    int err2 = bind(listen_socket, results->ai_addr, results->ai_addrlen);
    if (err2 == -1) {
        print_error(-1, "Server: Unable To Bind Socket");
        return -1;
    } else printf("Server: Successfully Binded Socket");

    // Setting Up Listen
    int err3 = listen(listen_socket, 10);
    if (err3 == -1) {
        print_error(-1, "Server: Unable To Set Up Listen");
        return -1;
    } else printf("Server: Successfully Set Up Listen");

    // Exiting Function
    free(hints);
    freeaddrinfo(results);
    return listen_socket;
}

int server_handshake(int listen_socket) {
    // Variable Declarations
    int client_socket;
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);

    // Accepting Connection
    client_socket = accept(listen_socket, (struct sockaddr *)&client_address, &sock_size);
    if (client_socket == -1) {
        print_error(-1, "Server: Unable To Accept Connection");
        return -1;
    } else printf("Server: Successfully Accepted Connection");

    // Exiting Function
    return client_socket;
}

void server_reset(int client_socket) {
    // Closing Socket
    int err = close(client_socket);
    if (err == -1) {
        print_error(-1, "Server: Unable To Reset Client Socket");
        return;
    } else printf("Server: Successfully Reset Client Socket");

    // Exiting Function
    return;
}

int client_handshake() {
    // Setting Up Hints Struct
    struct addrinfo *hints;
    hints = calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_INET; // IPv4
    hints->ai_socktype = SOCK_STREAM; // TCP Socket

    // Getting IP Information
    struct addrinfo *results;
    int err1 = getaddrinfo("127.0.0.1", "9845", hints, &results);
    if (err1 == -1) {
        print_error(-1, "Client: Unable To Get IP Information");
        return -1;
    } else printf("Client: Successfully Got IP Information");

    // Create Socket
    int server_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    if (server_socket == -1) {
        print_error(-1, "Client: Unable To Create Socket");
        return -1;
    } else printf("Client: Successfully Created Socket");

    // Connecting To Server
    int err2 = connect(server_socket, results->ai_addr, results->ai_addrlen);
    if (err2 == -1) {
        print_error(-1, "Client: Unable To Connect To Server");
        return -1;
    } printf("Client: Successfully Connected To Server");

    // Exiting Function
    free(hints);
    freeaddrinfo(results);
    return server_socket;
}
