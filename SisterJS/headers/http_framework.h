#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MAX_HEADERS 20
#define MAX_QUERY_PARAMS 20

// Enum for HTTP methods
typedef enum {
    GET,
    POST,
    PUT,
    DELETE,
    UNKNOWN
} HTTPMethod;

// Structure to hold HTTP header data
typedef struct {
    char name[256];
    char value[1024];
} HTTPHeader;

// Structure to hold HTTP query parameter
typedef struct {
    char key[256];
    char value[256];
} HTTPQueryParameter;

// Structure to hold HTTP request data
typedef struct {
    // First line
    HTTPMethod method;
    char path[256];
    char version[16];

    // Headers
    HTTPHeader headers[MAX_HEADERS];
    int header_count;

    // Query parameters
    HTTPQueryParameter query_params[MAX_QUERY_PARAMS];
    int query_param_count;
    
    // Body
    char body[BUFFER_SIZE];
} HTTPRequest;

// Structure to hold HTTP response data
typedef struct {
    // First line
    int status_code;
    char status_message[64];

    // Headers
    HTTPHeader headers[MAX_HEADERS];
    int header_count;

    // Body
    char body[BUFFER_SIZE];
} HTTPResponse;


void get_header_accept(HTTPRequest* req, char* accept) {
    for (int i = 0; i < req->header_count; i++) {
        if (strcmp(req->headers[i].name, "Accept") == 0) {
            strcpy(accept, req->headers[i].value);
            return;
        }
    }
    strcpy(accept, "*/*"); 
}

void get_header_content_type(HTTPRequest* req, char* content_type) {
    for (int i = 0; i < req->header_count; i++) {
        if (strcmp(req->headers[i].name, "Content-Type") == 0) {
            strcpy(content_type, req->headers[i].value);
            return;
        }
    }
    strcpy(content_type, "text/plain");
}

int get_param(HTTPRequest* req, const char* key, char* value) {
    for (int i = 0; i < req->query_param_count; i++) {
        if (strcmp(req->query_params[i].key, key) == 0) {
            strcpy(value, req->query_params[i].value);
            return 1;
        }
    }
    return 0;
}