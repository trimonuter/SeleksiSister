#pragma once

#include "http_framework.h"

void start_server(int port) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    HTTPRequest request;
    HTTPResponse response;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 10);

    printf("Server listening on port %d\n", port);

    while (1) {
        socklen_t addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            perror("recv failed");
            close(client_socket);
            continue;
        }
        // Null-terminate the buffer to prevent garbage values
        buffer[bytes_received] = '\0';

        // Print the raw HTTP request
        printf("\nReceived HTTP request:\n%s\n", buffer);

        // Parse and process request
        parse_request(buffer, &request);
        memset(&response, 0, sizeof(response));
        route_dispatcher(&request, &response);

        // Build and send response
        char response_buffer[BUFFER_SIZE];
        build_response(&response, response_buffer);
        printf("Sending HTTP response:\n%s\n", response_buffer);
        send(client_socket, response_buffer, strlen(response_buffer), 0);

        // Close the client socket
        close(client_socket);
    }

    close(server_socket);
}
