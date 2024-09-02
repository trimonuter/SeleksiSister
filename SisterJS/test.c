#include "headers/http_framework.h"

// Query parameter parser
void parse_query_params(char *path, HTTPRequest *request) {
    char path_copy[256];
    strcpy(path_copy, path);

    // Find query parameters
    char *query = strchr(path_copy, '?');
    if (query == NULL) {
        // strcpy(request->path, path);
        request->query_param_count = 0;
        return;
    }

    // Parse query parameters
    query++;
    int i = 0;
    while (*query != '\0') {
        while (*query != '=') {
            request->query_params[i].key[strlen(request->query_params[i].key)] = *query;
            query++;
        }
        query++;
        while (*query != '&' && *query != '\0') {
            request->query_params[i].value[strlen(request->query_params[i].value)] = *query;
            query++;
        }
        i++;
        if (*query == '&') query++;
    }

    request->query_param_count = i;

    // Remove query parameters from path
    // *query = '\0';

    // Copy path to request
    // strcpy(request->path, path);
}

// Request parser
void parse_request(char *buffer, HTTPRequest *request) {
    const char* delim = "\r\n";
    // Parse request first line
    char *line = strtok(buffer, delim);
    char method_buffer[16];

    sscanf(line, "%s %s %s", method_buffer, request->path, request->version);

    // Correct path if it contains query parameters
    char full_path[256];
    strcpy(full_path, request->path);

    for (int i = 0; i < strlen(request->path); i++) {
        if (request->path[i] == '?') {
            request->path[i] = '\0';
            break;
        }
    }

    // Convert method to enum
    if (strcmp(method_buffer, "GET") == 0) {
        request->method = GET;
    } else if (strcmp(method_buffer, "POST") == 0) {
        request->method = POST;
    } else if (strcmp(method_buffer, "PUT") == 0) {
        request->method = PUT;
    } else if (strcmp(method_buffer, "DELETE") == 0) {
        request->method = DELETE;
    } else {
        request->method = UNKNOWN;
    }

    // Parse query parameters
    parse_query_params(full_path, request);

    for (int i = 0; i < request->query_param_count; i++) {
        printf("Query param: %s=%s\n", request->query_params[i].key, request->query_params[i].value);
    }

    // Parse headers
    request->header_count = 0;
    int fix = 0;
    while ((line = strtok(NULL, "\r\n")) && strlen(line) > 0) {
        if (*(line - 2) == '\r') {
            fix = 1;
            break;
        }
        sscanf(
            line, 
            "%[^:]: %[^\r\n]", 
            request->headers[request->header_count].name,
            request->headers[request->header_count].value
        );
        request->header_count++;
    }

    if (fix) {
        char* line_copy = line;
        while (*line_copy != '\0') line_copy++;
        *line_copy = '\r';
    }
    strcpy(request->body, line);

    printf("Header count: %d\n", request->header_count);
    for (int i = 0; i < request->header_count; i++) {
        printf("Header: %s=%s\n", request->headers[i].name, request->headers[i].value);
    }
}


int main() {
    HTTPRequest req;
    char buffer[1024] = 
    "POST /students HTTP/1.1\r\nAccept: application/json\r\nContent-Type: application/json\r\nUser-Agent: PostmanRuntime/7.41.2\r\nPostman-Token: d130dbdc-10fe-4174-a430-6bf7c8812408\r\nHost: localhost:8080\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Length: 136\r\n\r\n{\r\n\"name\":\"Bob\",\r\n\"student_id\":\"1\",\r\n\"department\":\"Computer Science\",\r\n\"gpa\":\"3.1\",\r\n\"email\":\"bob@lmao.gmail.com\"\r\n}";
    parse_request(buffer, &req);
    return 0;
}