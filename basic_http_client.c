#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")

#define BUFFER_SIZE 8192
#define DEFAULT_HTTP_PORT 80
#define DEFAULT_HTTPS_PORT 443

// Initialize Winsock
void initialize_winsock() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "Winsock initialization failed. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}

// Create and connect socket
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

// Perform HTTPS connection using OpenSSL
SSL *initialize_ssl(SOCKET sock, const char *host) {
    SSL_library_init();
    SSL_load_error_strings();
    const SSL_METHOD *method = TLS_client_method();
    SSL_CTX *ctx = SSL_CTX_new(method);

    if (ctx == NULL) {
        fprintf(stderr, "SSL_CTX initialization failed.\n");
        exit(EXIT_FAILURE);
    }

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);
    SSL_set_tlsext_host_name(ssl, host);

    if (SSL_connect(ssl) <= 0) {
        fprintf(stderr, "SSL connection failed.\n");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Connected using %s encryption\n", SSL_get_cipher(ssl));
    return ssl;
}

// Send HTTP/HTTPS request
void send_request(SOCKET sock, SSL *ssl, const char *host, const char *path, int use_ssl, const char *headers) {
    char request[2048];
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "%s"
             "Connection: close\r\n\r\n",
             path, host, headers);

    if (use_ssl) {
        if (SSL_write(ssl, request, strlen(request)) <= 0) {
            fprintf(stderr, "Failed to send HTTPS request.\n");
            SSL_free(ssl);
            closesocket(sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
    } else {
        if (send(sock, request, strlen(request), 0) == SOCKET_ERROR) {
            fprintf(stderr, "Failed to send HTTP request. Error Code: %d\n", WSAGetLastError());
            closesocket(sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
    }
}

// Receive and process HTTP/HTTPS response
void receive_response(SOCKET sock, SSL *ssl, int use_ssl, int save_to_file) {
    char buffer[BUFFER_SIZE];
    int bytes_received;
    FILE *output_file = NULL;

    if (save_to_file) {
        output_file = fopen("response.txt", "w");
        if (output_file == NULL) {
            fprintf(stderr, "Failed to open file for saving response.\n");
            return;
        }
    }

    printf("HTTP Response:\n");

    do {
        if (use_ssl) {
            bytes_received = SSL_read(ssl, buffer, sizeof(buffer) - 1);
        } else {
            bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        }

        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            if (save_to_file) {
                fprintf(output_file, "%s", buffer);
            } else {
                printf("%s", buffer);
            }
        }
    } while (bytes_received > 0);

    if (save_to_file) {
        printf("\nResponse saved to response.txt\n");
        fclose(output_file);
    }

    if (bytes_received < 0) {
        fprintf(stderr, "Error receiving response.\n");
    }
}

int main() {
    char host[256];
    char path[256] = "/";
    int port;
    char protocol[6];
    int use_ssl = 0;
    char headers[1024] = "";

    // User input
    printf("Enter protocol (http or https): ");
    scanf("%5s", protocol);
    use_ssl = (strcmp(protocol, "https") == 0);

    printf("Enter hostname (e.g., example.com): ");
    scanf("%255s", host);

    printf("Enter path (e.g., /index.html) [default is '/']: ");
    getchar(); // Clear newline
    fgets(path, sizeof(path), stdin);
    path[strcspn(path, "\n")] = 0; // Remove newline
    if (strlen(path) == 0) {
        strcpy(path, "/");
    }

    port = use_ssl ? DEFAULT_HTTPS_PORT : DEFAULT_HTTP_PORT;

    printf("Enter custom headers (e.g., User-Agent: MyClient) [Press Enter to skip]: ");
    fgets(headers, sizeof(headers), stdin);
    headers[strcspn(headers, "\n")] = 0; // Remove newline

    // Initialize Winsock
    initialize_winsock();

    // Create and connect socket
    SOCKET sock = create_and_connect_socket(host, port);

    // Initialize SSL if needed
    SSL *ssl = NULL;
    if (use_ssl) {
        ssl = initialize_ssl(sock, host);
    }

    // Send request
    send_request(sock, ssl, host, path, use_ssl, headers);

    // Receive response
    int save_to_file = 0;
    printf("Save response to file? (1 for yes, 0 for no): ");
    scanf("%d", &save_to_file);

    receive_response(sock, ssl, use_ssl, save_to_file);

    // Cleanup
    if (use_ssl) {
        SSL_free(ssl);
    }
    closesocket(sock);
    WSACleanup();
    return 0;
}
