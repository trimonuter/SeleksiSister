#include "headers/parser.h"
#include "headers/routing.h"
#include "headers/server.h"
#include "headers/json.h"

#include <dirent.h>

#define LINE_BUFFER_SIZE 256
#define FILEPATH_BUFFER_SIZE 512
#define MAX_STUDENTS 100
#define STUDENT_DATA_SIZE 7 * LINE_BUFFER_SIZE

typedef struct {
    char name [LINE_BUFFER_SIZE];
    char student_id [LINE_BUFFER_SIZE];
    char department [LINE_BUFFER_SIZE];
    char gpa [LINE_BUFFER_SIZE];
    char email [LINE_BUFFER_SIZE];
} Student;

// Get students
int read_student_file(const char* filepath, Student* student, HTTPRequest* req) {
    // Open student file
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read student data
    char name[LINE_BUFFER_SIZE]; memset(name, 0, LINE_BUFFER_SIZE);
    char student_id[LINE_BUFFER_SIZE]; memset(student_id, 0, LINE_BUFFER_SIZE);
    char department[LINE_BUFFER_SIZE]; memset(department, 0, LINE_BUFFER_SIZE);
    char gpa[LINE_BUFFER_SIZE]; memset(gpa, 0, LINE_BUFFER_SIZE);
    char email[LINE_BUFFER_SIZE]; memset(email, 0, LINE_BUFFER_SIZE);

    fgets(name, LINE_BUFFER_SIZE, file);
    fgets(student_id, LINE_BUFFER_SIZE, file);
    fgets(department, LINE_BUFFER_SIZE, file);
    fgets(gpa, LINE_BUFFER_SIZE, file);
    fgets(email, LINE_BUFFER_SIZE, file);

    // Remove newline characters
    name[strcspn(name, "\n")] = 0;
    student_id[strcspn(student_id, "\n")] = 0;
    department[strcspn(department, "\n")] = 0;
    gpa[strcspn(gpa, "\n")] = 0;
    email[strcspn(email, "\n")] = 0;

    name[strcspn(name, "\r")] = 0;
    student_id[strcspn(student_id, "\r")] = 0;
    department[strcspn(department, "\r")] = 0;
    gpa[strcspn(gpa, "\r")] = 0;
    email[strcspn(email, "\r")] = 0;

    // Return 1 if student data doesn't match param
    char param_name[256];           memset(param_name, 0, 256);
    char param_student_id[256];     memset(param_student_id, 0, 256);
    char param_department[256];     memset(param_department, 0, 256);
    char param_gpa[256];            memset(param_gpa, 0, 256);
    char param_email[256];          memset(param_email, 0, 256);

    if (get_param(req, "name", param_name)) {
        if (strcmp(name, param_name) != 0) goto fail;
    }
    if (get_param(req, "student_id", param_student_id)) {
        if (strcmp(student_id, param_student_id) != 0) goto fail;
    }
    if (get_param(req, "department", param_department)) {
        if (strcmp(department, param_department) != 0) goto fail;
    }
    if (get_param(req, "gpa", param_gpa)) {
        if (strcmp(gpa, param_gpa) != 0) goto fail;
    }
    if (get_param(req, "email", param_email)) {
        if (strcmp(email, param_email) != 0) goto fail;
    }

    // Copy student data
    strcpy(student->name, name);
    strcpy(student->student_id, student_id);
    strcpy(student->department, department);
    strcpy(student->gpa, gpa);
    strcpy(student->email, email);

    // Close file
    success:
        fclose(file);
        return 0;
    fail:
        fclose(file);
        return 1;
}

void read_students(const char* path, Student* students, int* student_count, HTTPRequest* req) {
    // Open directory
    DIR* dir = opendir(path);    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    // Read student files
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip the current directory (.) and parent directory (..), and skip format.txt
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, "format.txt") == 0) {
            continue;
        }

        // Build filepath
        char filepath[FILEPATH_BUFFER_SIZE]; memset(filepath, 0, FILEPATH_BUFFER_SIZE);
        snprintf(filepath, FILEPATH_BUFFER_SIZE, "%s/%s", path, entry->d_name);

        // Read student data
        int add = read_student_file(filepath, &students[*student_count], req);
        if (!add) (*student_count)++;
    }

    closedir(dir);
}

void get_students_handler(HTTPRequest* req, HTTPResponse* res) {
    res->status_code = 200;
    strcpy(res->status_message, "OK");

    // Read student data
    Student students[MAX_STUDENTS]; memset(students, 0, sizeof(students));
    int student_count = 0;

    read_students("./university_data/students", students, &student_count, req);
    
    // Get accept type
    char accept[256];
    get_header_accept(req, accept);

    // Build response
    if (strcmp(accept, "application/json") == 0) {
        // Build response json
        JSONObject json;
        json.attr_count = 0;

        json_add_attr(&json, "status", "success");
        json_add_attr(&json, "message", "Students data retrieved successfully");
        
        // Get student data
        char data[STUDENT_DATA_SIZE * student_count]; memset(data, 0, STUDENT_DATA_SIZE * student_count);
        strcpy(data, "[\n");

        for (int i = 0; i < student_count; i++) {
            // Build student json
            JSONObject student_json; memset(&student_json, 0, sizeof(student_json));
            student_json.attr_count = 0;

            json_add_attr(&student_json, "name", students[i].name);
            json_add_attr(&student_json, "student_id", students[i].student_id);
            json_add_attr(&student_json, "department", students[i].department);
            json_add_attr(&student_json, "gpa", students[i].gpa);
            json_add_attr(&student_json, "email", students[i].email);

            // Stringify student json
            char student_json_str[STUDENT_DATA_SIZE]; memset(student_json_str, 0, STUDENT_DATA_SIZE);
            json_stringify(&student_json, student_json_str, 1, 2);

            // Append student json to data
            if (i > 0) strcat(data, ",\n");
            strcat(data, student_json_str);
        }
        strcat(data, "]");

        // Add data to json
        json_add_attr(&json, "data", data);

        // Stringify json
        char body[STUDENT_DATA_SIZE * student_count + 1024]; memset(body, 0, STUDENT_DATA_SIZE * student_count + 1024);
        json_stringify(&json, body, 1, 0);

        strcpy(res->body, body);
    } else {
        char body[STUDENT_DATA_SIZE * student_count]; memset(body, 0, STUDENT_DATA_SIZE * student_count);
        strcpy(body, "");

        for (int i = 0; i < student_count; i++) {
            char student_data[STUDENT_DATA_SIZE]; memset(student_data, 0, STUDENT_DATA_SIZE);
            snprintf(student_data, STUDENT_DATA_SIZE, "Name: %s\nStudent ID: %s\nDepartment: %s\nGPA: %s\nEmail: %s\n\n", students[i].name, students[i].student_id, students[i].department, students[i].gpa, students[i].email);
            strcat(body, student_data);
        }

        strcpy(res->body, body);
    }
}

void post_students_handler(HTTPRequest* req, HTTPResponse* res) {
    res->status_code = 200;
    strcpy(res->status_message, "OK");

    // Open students directory
    DIR* dir = opendir("./university_data/students");
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    // Get student count
    struct dirent* entry;
    int student_count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, "format.txt") == 0) {
            continue;
        }
        student_count++;
    }

    closedir(dir);

    // Get content type
    char content_type[256];
    get_header_content_type(req, content_type);

    // Submission handler
    int content_json = strcmp(content_type, "application/json") == 0;
    int content_x_www_form = strcmp(content_type, "application/x-www-form-urlencoded") == 0;
    if (content_json || content_x_www_form) {
        // Parse JSON
        JSONObject json;
        memset(&json, 0, sizeof(json));

        json.attr_count = 0;
        if (content_json)   json_parse(&json, req->body);
        else                query_parse(&json, req->body);

        // Get student data
        char name[LINE_BUFFER_SIZE];
        char student_id[LINE_BUFFER_SIZE];
        char department[LINE_BUFFER_SIZE];
        char gpa[LINE_BUFFER_SIZE];
        char email[LINE_BUFFER_SIZE];

        memset(name, 0, LINE_BUFFER_SIZE);
        memset(student_id, 0, LINE_BUFFER_SIZE);
        memset(department, 0, LINE_BUFFER_SIZE);
        memset(gpa, 0, LINE_BUFFER_SIZE);
        memset(email, 0, LINE_BUFFER_SIZE);

        for (int i = 0; i < json.attr_count; i++) {
            if (strcmp(json.attr[i].key, "name") == 0) strcpy(name, json.attr[i].value);
            if (strcmp(json.attr[i].key, "student_id") == 0) strcpy(student_id, json.attr[i].value);
            if (strcmp(json.attr[i].key, "department") == 0) strcpy(department, json.attr[i].value);
            if (strcmp(json.attr[i].key, "gpa") == 0) strcpy(gpa, json.attr[i].value);
            if (strcmp(json.attr[i].key, "email") == 0) strcpy(email, json.attr[i].value);
        }

        if (strcmp(student_id, "format") == 0) {
            res->status_code = 401;
            strcpy(res->status_message, "Unauthorized");
            strcpy(res->body, "401 Unauthorized: Cannot modify format.txt");
            return;
        }

        int has_student_id = strcmp(student_id, "") != 0;

        // Write student data
        char filepath[FILEPATH_BUFFER_SIZE]; memset(filepath, 0, FILEPATH_BUFFER_SIZE);
        if (has_student_id) {
            snprintf(filepath, FILEPATH_BUFFER_SIZE, "./university_data/students/%s.txt", student_id);

        } else {
            snprintf(filepath, FILEPATH_BUFFER_SIZE, "./university_data/students/%d.txt", student_count + 1);
        }

        if (access(filepath, F_OK) == 0) {
            res->status_code = 409;
            strcpy(res->status_message, "Conflict");
            strcpy(res->body, "409 Conflict: Student ID already exists");
            return;
        }

        FILE* file = fopen(filepath, "w");
        if (file == NULL) {
            perror("Error opening file");
            return;
        }

        fprintf(file, "%s\n", name);
        fprintf(file, "%s\n", student_id);
        fprintf(file, "%s\n", department);
        fprintf(file, "%s\n", gpa);
        fprintf(file, "%s", email);

        fclose(file);

        // Send response
        char accept[256];
        memset(accept, 0, 256);
        get_header_accept(req, accept);

        if (strcmp(accept, "application/json") == 0) {
            // Build response json
            JSONObject response_json;
            memset(&response_json, 0, sizeof(response_json));
            response_json.attr_count = 0;

            json_add_attr(&response_json, "status", "success");
            json_add_attr(&response_json, "message", "Student data added successfully");

            JSONObject student_json;
            memset(&student_json, 0, sizeof(student_json));
            student_json.attr_count = 0;

            json_add_attr(&student_json, "name", name);
            json_add_attr(&student_json, "student_id", student_id);
            json_add_attr(&student_json, "department", department);
            json_add_attr(&student_json, "gpa", gpa);
            json_add_attr(&student_json, "email", email);

            // Stringify student json
            char student_json_str[STUDENT_DATA_SIZE];
            memset(student_json_str, 0, STUDENT_DATA_SIZE);
            json_stringify(&student_json, student_json_str, 1, 2);

            // Add student data to response json
            json_add_attr(&response_json, "data", student_json_str);

            // Stringify response json
            char body[STUDENT_DATA_SIZE + 1024];
            json_stringify(&response_json, body, 1, 0);

            strcpy(res->body, body);
        } else {
            char body[2048];
            memset(body, 0, 2048);

            snprintf(body, 2048, "Student data added successfully.\n\nName: %s\nStudent ID: %s\nDepartment: %s\nGPA: %s\nEmail: %s\n", name, student_id, department, gpa, email);
            strcpy(res->body, body);
        }
    } else {
        // Write student data
        char filepath[FILEPATH_BUFFER_SIZE]; memset(filepath, 0, FILEPATH_BUFFER_SIZE);
        snprintf(filepath, FILEPATH_BUFFER_SIZE, "./university_data/students/%d.txt", student_count + 1);

        if (access(filepath, F_OK) == 0) {
            res->status_code = 409;
            strcpy(res->status_message, "Conflict");
            strcpy(res->body, "409 Conflict: Student ID already exists");
            return;
        }

        FILE* file = fopen(filepath, "w");
        if (file == NULL) {
            perror("Error opening file");
            return;
        }

        fprintf(file, "%s", req->body);

        fclose(file);

        // Send resposne
        char body[2048];
        memset(body, 0, 2048);

        strcpy(body, "A student data have been successfully created in the university database as follows:\n\n");
        strcat(body, req->body);

        // Get accept type
        char accept[256];
        memset(accept, 0, 256);
        get_header_accept(req, accept);

        if (strcmp(accept, "application/json") == 0) {
            // Build response json
            JSONObject json;
            memset(&json, 0, sizeof(json));
            json.attr_count = 0;

            json_add_attr(&json, "status", "success");
            json_add_attr(&json, "message", "Student data added successfully");
            json_add_attr(&json, "data", req->body);

            // Stringify json
            char json_str[2048];
            memset(json_str, 0, 2048);
            json_stringify(&json, json_str, 1, 0);

            strcpy(res->body, json_str);
        } else {
            strcpy(res->body, body);
        }
    }
}

void put_students_handler(HTTPRequest* req, HTTPResponse* res) {
    res->status_code = 200;
    strcpy(res->status_message, "OK");

    // Get content type
    char content_type[256]; memset(content_type, 0, 256);
    get_header_content_type(req, content_type);

    // Submission handler
    int content_json = strcmp(content_type, "application/json") == 0;
    int content_x_www_form = strcmp(content_type, "application/x-www-form-urlencoded") == 0;
    if (content_json || content_x_www_form) {
        // Parse JSON
        JSONObject json;
        memset(&json, 0, sizeof(json));

        json.attr_count = 0;
        if (content_json)   json_parse(&json, req->body);
        else                query_parse(&json, req->body);

        // Get student data
        char name[LINE_BUFFER_SIZE]; memset(name, 0, LINE_BUFFER_SIZE);
        char student_id[LINE_BUFFER_SIZE]; memset(student_id, 0, LINE_BUFFER_SIZE);
        char department[LINE_BUFFER_SIZE]; memset(department, 0, LINE_BUFFER_SIZE);
        char gpa[LINE_BUFFER_SIZE]; memset(gpa, 0, LINE_BUFFER_SIZE);
        char email[LINE_BUFFER_SIZE]; memset(email, 0, LINE_BUFFER_SIZE);

        for (int i = 0; i < json.attr_count; i++) {
            if (strcmp(json.attr[i].key, "name") == 0) strcpy(name, json.attr[i].value);
            if (strcmp(json.attr[i].key, "student_id") == 0) strcpy(student_id, json.attr[i].value);
            if (strcmp(json.attr[i].key, "department") == 0) strcpy(department, json.attr[i].value);
            if (strcmp(json.attr[i].key, "gpa") == 0) strcpy(gpa, json.attr[i].value);
            if (strcmp(json.attr[i].key, "email") == 0) strcpy(email, json.attr[i].value);
        }

        if (strcmp(student_id, "format") == 0) {
            res->status_code = 401;
            strcpy(res->status_message, "Unauthorized");
            strcpy(res->body, "401 Unauthorized: Cannot modify format.txt");
            return;
        }

        if (strcmp(student_id, "") == 0) {
            res->status_code = 400;
            strcpy(res->status_message, "Bad Request");
            strcpy(res->body, "400 Bad Request: Missing student ID");
            return;
        }

        // Write student data
        char filepath[FILEPATH_BUFFER_SIZE]; memset(filepath, 0, FILEPATH_BUFFER_SIZE);
        snprintf(filepath, FILEPATH_BUFFER_SIZE, "./university_data/students/%s.txt", student_id);

        if (access(filepath, F_OK) != 0) {
            res->status_code = 404;
            strcpy(res->status_message, "Not Found");
            strcpy(res->body, "404 Not Found: Student ID not found");
            return;
        }

        FILE* file = fopen(filepath, "w");
        if (file == NULL) {
            perror("Error opening file");
            return;
        }

        fprintf(file, "%s\n", name);
        fprintf(file, "%s\n", student_id);
        fprintf(file, "%s\n", department);
        fprintf(file, "%s\n", gpa);
        fprintf(file, "%s", email);

        fclose(file);

        // Send response
        char accept[256]; memset(accept, 0, 256);
        get_header_accept(req, accept);

        if (strcmp(accept, "application/json") == 0) {
            // Build response json
            JSONObject response_json;
            memset(&response_json, 0, sizeof(response_json));
            response_json.attr_count = 0;

            json_add_attr(&response_json, "status", "success");
            json_add_attr(&response_json, "message", "Student data updated successfully");

            JSONObject student_json;
            memset(&student_json, 0, sizeof(student_json));
            student_json.attr_count = 0;

            json_add_attr(&student_json, "name", name);
            json_add_attr(&student_json, "student_id", student_id);
            json_add_attr(&student_json, "department", department);
            json_add_attr(&student_json, "gpa", gpa);
            json_add_attr(&student_json, "email", email);

            // Stringify student json
            char student_json_str[STUDENT_DATA_SIZE]; memset(student_json_str, 0, STUDENT_DATA_SIZE);
            json_stringify(&student_json, student_json_str, 1, 2);

            // Add student data to response json
            json_add_attr(&response_json, "data", student_json_str);

            // Stringify response json
            char body[STUDENT_DATA_SIZE + 1024]; memset(body, 0, STUDENT_DATA_SIZE + 1024);
            json_stringify(&response_json, body, 1, 0);

            strcpy(res->body, body);
        } else {
            char body[2048]; memset(body, 0, 2048);
            snprintf(body, 2048, "Student data with ID %s has been successfully updated in the university database as follows:\n\nName: %s\nStudent ID: %s\nDepartment: %s\nGPA: %s\nEmail: %s\n", student_id, name, student_id, department, gpa, email);
            strcpy(res->body, body);
        }
    } else {
        char id[256]; memset(id, 0, 256);
        int has_id = get_param(req, "id", id);

        if (strcmp(id, "format") == 0) {
            res->status_code = 401;
            strcpy(res->status_message, "Unauthorized");
            strcpy(res->body, "401 Unauthorized: Cannot modify format.txt");
            return;
        }

        if (!has_id) {
            res->status_code = 400;
            strcpy(res->status_message, "Bad Request");
            strcpy(res->body, "400 Bad Request: Missing student ID");
            return;
        }

        // Open student file
        char filepath[FILEPATH_BUFFER_SIZE]; memset(filepath, 0, FILEPATH_BUFFER_SIZE);
        snprintf(filepath, FILEPATH_BUFFER_SIZE, "./university_data/students/%s.txt", id);

        if (access(filepath, F_OK) != 0) {
            res->status_code = 404;
            strcpy(res->status_message, "Not Found");
            strcpy(res->body, "404 Not Found: Student ID not found");
            return;
        }

        FILE* file = fopen(filepath, "w");
        if (file == NULL) {
            perror("Error opening file");
            return;
        }

        // Overwrite student data
        fprintf(file, "%s", req->body);
        fclose(file);

        // Send response
        char body[2048]; memset(body, 0, 2048);
        snprintf(body, 2048, "Student data with ID %s has been successfully updated in the university database as follows:\n\n%s", id, req->body);

        char accept[256]; memset(accept, 0, 256);
        get_header_accept(req, accept);

        if (strcmp(accept, "application/json") == 0) {
            // Build response json
            JSONObject json;
            memset(&json, 0, sizeof(json));
            json.attr_count = 0;

            json_add_attr(&json, "status", "success");
            json_add_attr(&json, "message", "Student data updated successfully");
            json_add_attr(&json, "data", req->body);

            // Stringify json
            char json_str[2048]; memset(json_str, 0, 2048);
            json_stringify(&json, json_str, 1, 0);

            strcpy(res->body, json_str);
        } else {
            strcpy(res->body, body);
        }
    }
}

void delete_students_handler(HTTPRequest* req, HTTPResponse* res) {
    res->status_code = 200;
    strcpy(res->status_message, "OK");

    // Get student ID
    char id[256]; memset(id, 0, 256);
    int has_id = get_param(req, "id", id);

    if (strcmp(id, "format") == 0) {
        res->status_code = 401;
        strcpy(res->status_message, "Unauthorized");
        strcpy(res->body, "401 Unauthorized: Cannot modify format.txt");
        return;
    }

    if (!has_id) {
        res->status_code = 400;
        strcpy(res->status_message, "Bad Request");
        strcpy(res->body, "400 Bad Request: Missing student ID");
        return;
    }

    // Delete student file
    char filepath[FILEPATH_BUFFER_SIZE]; memset(filepath, 0, FILEPATH_BUFFER_SIZE);
    snprintf(filepath, FILEPATH_BUFFER_SIZE, "./university_data/students/%s.txt", id);
    
    if (access(filepath, F_OK) != 0) {
        res->status_code = 404;
        strcpy(res->status_message, "Not Found");
        strcpy(res->body, "404 Not Found: Student ID not found");
        return;
    }

    remove(filepath);

    // Send response
    char body[2048]; memset(body, 0, 2048);
    snprintf(body, 2048, "Student data with ID %s has been successfully deleted from the university database.", id);

    char accept[256]; memset(accept, 0, 256);
    get_header_accept(req, accept);

    if (strcmp(accept, "application/json") == 0) {
        // Build response json
        JSONObject json;
        memset(&json, 0, sizeof(json));
        json.attr_count = 0;

        json_add_attr(&json, "status", "success");
        json_add_attr(&json, "message", "Student data deleted successfully");

        // Stringify json
        char json_str[2048]; memset(json_str, 0, 2048);
        json_stringify(&json, json_str, 1, 0);

        strcpy(res->body, json_str);
    } else {
        strcpy(res->body, body);
    }
}

// Main server code
int main() {
    add_route(GET, "/students", get_students_handler);
    add_route(POST, "/students", post_students_handler);
    add_route(PUT, "/students", put_students_handler);
    add_route(DELETE, "/students", delete_students_handler);
    
    start_server(8080);
    return 0;
}

// int main() {
//     char buffer[2048] =
//     "POST /students HTTP/1.1\r\nAccept: application/json\r\nContent-Type: application/json\r\nUser-Agent: PostmanRuntime/7.41.2\r\nPostman-Token: d130dbdc-10fe-4174-a430-6bf7c8812408\r\nHost: localhost:8080\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Length: 136\r\n\r\n{\r\n\"name\":\"Bob\",\r\n\"student_id\":\"1\",\r\n\"department\":\"Computer Science\",\r\n\"gpa\":\"3.1\",\r\n\"email\":\"bob@lmao.gmail.com\"\r\n}";

//     HTTPRequest req;
//     HTTPResponse res;

//     parse_request(buffer, &req);
//     post_students_handler(&req, &res);
// }