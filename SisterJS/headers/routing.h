#pragma once

#include "http_framework.h"

// Define the maximum number of routes
#define MAX_ROUTES 10

// Structure to hold a route
typedef struct {
    // Route path
    char path[256];

    // Route HTTP method
    HTTPMethod method;

    // Route handler function
    void (*handler)(HTTPRequest *request, HTTPResponse *response);
} Route;

// Route table
Route routes[MAX_ROUTES];
int route_count = 0;

// Function to register a route
void add_route(HTTPMethod method, const char *path, void (*handler)(HTTPRequest *request, HTTPResponse *response)) {
    if (route_count < MAX_ROUTES) {
        // Request method
        routes[route_count].method = method;
        
        // Request path
        strcpy(routes[route_count].path, path);
        
        // Request handler
        routes[route_count].handler = handler;
        
        // Increment route count
        route_count++;
    }
}

// Function to find and dispatch the appropriate route handler
void route_dispatcher(HTTPRequest *request, HTTPResponse *response) {
    // Error 405: Method Not Allowed
    if (request->method == UNKNOWN) {
        // Response status code
        response->status_code = 405;
        
        // Response status message
        strcpy(response->status_message, "Method Not Allowed");

        // Response headers
        response->headers[response->header_count] = (HTTPHeader) {
            .name = "Allow",
            .value = "GET, POST, PUT, DELETE"
        };

        // Response body
        strcpy(response->body, "405 Method Not Allowed");
        return;
    }

    // Find the route handler
    for (int i = 0; i < route_count; i++) {
        if (strcmp(request->path, routes[i].path) == 0 && request->method == routes[i].method) {
            routes[i].handler(request, response);
            return;
        }
    }

    // Default 404 handler if no route matches
    response->status_code = 404;
    strcpy(response->status_message, "Not Found");
    strcpy(response->body, "404 Page Not Found");
}
