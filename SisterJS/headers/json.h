#pragma once

#include "http_framework.h"

// Define the maximum number of JSON attributes
#define MAX_JSON_ATTR 20

// Structure for JSON attributes
typedef struct {
    char key[256];
    char value[4096];
} JSONAttribute;

// Structure for JSON object
typedef struct {
    JSONAttribute attr[MAX_JSON_ATTR];
    int attr_count;
} JSONObject;

// Function to add a JSON attribute to a JSON object
void json_add_attr(JSONObject *json, const char *key, const char *value) {
    if (json->attr_count < MAX_JSON_ATTR) {
        strcpy(json->attr[json->attr_count].key, key);
        strcpy(json->attr[json->attr_count].value, value);
        json->attr_count++;
    }
}

// Function to stringify a JSON object
void json_stringify(JSONObject *json, char *buffer, int newline, int indent) {
    strcpy(buffer, "");
    for (int i = 0; i < indent; i++) strcat(buffer, "\t"); 
    strcat(buffer, "{");
    if (newline) strcat(buffer, "\n");
    
    for (int i = 0; i < json->attr_count; i++) {
        if (newline) for (int j = 0; j < indent + 1; j++) strcat(buffer, "\t");
        strcat(buffer, "\"");
        strcat(buffer, json->attr[i].key);
        strcat(buffer, "\":\"");
        strcat(buffer, json->attr[i].value);
        strcat(buffer, "\"");

        if (i < json->attr_count - 1)   strcat(buffer, ",");
        if (newline)                    strcat(buffer, "\n");
    }
    for (int i = 0; i < indent; i++) strcat(buffer, "\t"); 
    strcat(buffer, "}");
} 

void json_parse(JSONObject *json, char *buffer) {
    while (*buffer != '\0') {
        // Increment until "
        while (*buffer != '\"') buffer++;
        buffer++;
        
        // Get key
        int len = 0;
        while (*buffer != '\"') {
            json->attr[json->attr_count].key[len] = *buffer;
            buffer++;
            len++;
        }
        buffer++;
        json->attr[json->attr_count].key[len] = '\0';

        // Increment until :
        while (*buffer != ':') buffer++;
        buffer++;

        // Increment until "
        while (*buffer != '\"') buffer++;
        buffer++;

        // Get value
        len = 0;
        while (*buffer != '\"') {
            json->attr[json->attr_count].value[len] = *buffer;
            buffer++;
            len++;
        }
        buffer++;
        json->attr[json->attr_count].value[len] = '\0';

        json->attr_count++;
        if (*buffer != ',') return;
    }
}

void query_parse(JSONObject *json, char *buffer) {
    while (*buffer != '\0') {
        int len = 0;
        while (*buffer != '=') {
            json->attr[json->attr_count].key[len] = *buffer;
            buffer++;
            len++;
        }

        buffer++;
        len = 0;
        while (*buffer != '&' && *buffer != '\0' && *buffer != '\r' && *buffer != '\n') {
            json->attr[json->attr_count].value[len] = *buffer;
            buffer++;
            len++;
        }

        json->attr_count++;
        if (*buffer == '&') buffer++;
    }
}