#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define BUFFER_SIZE 8192 // Size of the buffer for incoming data
#define DEFAULT_PORT 80  // Default HTTP port

// Function to initialize Winsock
void initialize_winsock() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "Winsock initialization failed. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}

// Function to create and connect a socket to the server
SOCKET create_and_connect_socket(const char *host, int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server;
    struct hostent *he;

    if ((he = gethostbyname(host)) == NULL) {
        fprintf(stderr, "Failed to resolve hostname %s.\n", host);
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    memcpy(&server.sin_addr, he->h_addr_list[0], he->h_length);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        fprintf(stderr, "Connection to server failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    return sock;
}

// Function to send an HTTP GET request
void send_http_request(SOCKET sock, const char *host, const char *path) {
    char request[1024];
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",
             path, host);

    if (send(sock, request, strlen(request), 0) == SOCKET_ERROR) {
        fprintf(stderr, "Failed to send HTTP request. Error Code: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

// Function to receive and print the HTTP response
void receive_http_response(SOCKET sock) {
    char *response = NULL;
    size_t response_size = 0;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    printf("HTTP Response:\n");

    // Receive data in chunks and dynamically allocate memory
    while ((bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';

        // Reallocate memory for the full response
        char *new_response = realloc(response, response_size + bytes_received + 1);
        if (new_response == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            free(response);
            closesocket(sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        response = new_response;
        memcpy(response + response_size, buffer, bytes_received);
        response_size += bytes_received;
        response[response_size] = '\0';
    }

    if (bytes_received == SOCKET_ERROR) {
        fprintf(stderr, "Error receiving response. Error Code: %d\n", WSAGetLastError());
    } else if (response) {
        printf("%s\n", response);
    } else {
        printf("No response received.\n");
    }

    free(response);
}

int main() {
    char host[256];
    char path[256] = "/";
    int port = DEFAULT_PORT;

    // Get user input
    printf("Enter hostname (e.g., example.com): ");
    scanf("%255s", host);

    printf("Enter path (e.g., /index.html) [default is '/']: ");
    getchar(); // Clear newline from buffer
    fgets(path, sizeof(path), stdin);
    path[strcspn(path, "\n")] = 0; // Remove newline

    if (strlen(path) == 0) {
        strcpy(path, "/"); // Default to root path
    }

    printf("Enter port [default is 80]: ");
    char port_input[10];
    fgets(port_input, sizeof(port_input), stdin);
    port_input[strcspn(port_input, "\n")] = 0; // Remove newline

    if (strlen(port_input) > 0) {
        port = atoi(port_input);
        if (port <= 0) {
            fprintf(stderr, "Invalid port number. Using default port %d.\n", DEFAULT_PORT);
            port = DEFAULT_PORT;
        }
    }

    // Initialize Winsock
    initialize_winsock();

    // Create and connect socket
    SOCKET sock = create_and_connect_socket(host, port);

    // Send HTTP GET request
    send_http_request(sock, host, path);

    // Receive and print HTTP response
    receive_http_response(sock);

    // Cleanup
    closesocket(sock);
    WSACleanup();
    return 0;
}
