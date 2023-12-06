#include <string.h>
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h>  /* socket, connect */
#include <netinet/in.h>  /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "parson.h"
#include "helpers.h"
#include "requests.h"

int code(char* cmd) {
    // Check if the command is "exit"
    if (strcmp(cmd, "exit") == 0) {
        return 0; 
        // Return code 0 to indicate "exit" command
    }
    // Check if the command is "register"
    if (strcmp(cmd, "register") == 0) {
        return 1; 
        // Return code 1 to indicate "register" command
    }
    // Check if the command is "login"
    if (strcmp(cmd, "login") == 0) {
        return 2; 
        // Return code 2 to indicate "login" command
    }
    // Check if the command is "enter_library"
    if (strcmp(cmd, "enter_library") == 0) {
        return 3; 
        // Return code 3 to indicate "enter_library" command
    }
    // Check if the command is "get_books"
    if (strcmp(cmd, "get_books") == 0) {
        return 4; 
        // Return code 4 to indicate "get_books" command
    }
    // Check if the command is "get_book"
    if (strcmp(cmd, "get_book") == 0) {
        return 5; 
        // Return code 5 to indicate "get_book" command
    }
    // Check if the command is "add_book"
    if (strcmp(cmd, "add_book") == 0) {
        return 6; 
        // Return code 6 to indicate "add_book" command
    }
    // Check if the command is "delete_book"
    if (strcmp(cmd, "delete_book") == 0) {
        return 7; 
        // Return code 7 to indicate "delete_book" command
    }
    // Check if the command is "logout"
    if (strcmp(cmd, "logout") == 0) {
        return 8; 
        // Return code 8 to indicate "logout" command
    }
    // If the command does not match any known commands, return -1
    return -1;
}

void remove_whitespace(char *str) {
    int len = strlen(str);  // Get the length of the input string
    int i, j = 0;  // Declare variables for iteration and new string index
    // Iterate over each character of the input string
    for (i = 0; i < len; i++) {
        // Check if the current character is not a whitespace or backspace
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n'
        && str[i] != '\r' && str[i] != '\b') {
            str[j++] = str[i];
            // Copy the non-whitespace character to the new string
        }
    }
    str[j] = '\0';
    // Add the null terminator at the end to terminate the new string
}

void extract_cookie_elements(const char *line, char **cookie,
int *n) {
    const char *sep = "; ";
    // Separator used to tokenize the line
    char *cpy = strdup(line);
    // Create a copy of the line not to loose it
    char *p = strtok(cpy, sep);
    // Tokenize the line using the separator
    *n = 0;
    // Initialize the number of elements to 0 for safety
    // Iterate over each token and check for the end condition
    while (p != NULL && strncmp(p, "Date: ", strlen("Date: ")) != 0) {
        while (*p == ' ') {
            p++;
            // Skip leading spaces
        }
        cookie[*n] = strdup(p);
        // Copy the token to the cookie elements array
        (*n)++;
        // Increment the number of elements
        p = strtok(NULL, sep);
        // Move to the next token
    }
    free(cpy);
    // Free the memory allocated for the line copy
}

char* get_last_line(const char* input, const char* target) {
    char* tofind = NULL;
    // Variable to store the last line containing the target
    char* line = strtok((char*)input, "\n");
    // Tokenize the input string by newline characters
    while (line != NULL) {
        if (strstr(line, target) != NULL) {
            // If the target is found in the current line,
            // update the tofind variable
            tofind = line;
        }
        line = strtok(NULL, "\n"); // Move to the next line
    }
    return tofind;
    // Return the char* containing the target, or NULL if not found
}

void print_fields(JSON_Array* array) {
    size_t count = json_array_get_count(array);
    // Get the number of elements in the JSON array
    // Iterate over each element in the array
    for (size_t i = 0; i < count; i++) {
        JSON_Object* object = json_array_get_object(array, i);
        // Get the object at the current index
        // Retrieve the values of the "id" and "title" fields from the object
        int id = json_object_get_number(object, "id");
        const char* title = json_object_get_string(object, "title");
        // Print the values of the fields
        printf("ID: %d, Title: %s\n", id, title);
    }
}

void exit_handler() {
    puts("204 - No Content.");
    // Print the message indicating no content
    exit(0);
    // Terminate the program with a successful status code (0)
}

void register_handler(char *message, char *response, int sockfd){
    char *usr = (char*)calloc(50, sizeof(char));
    // Allocate memory for the username
    char *passwd = (char*)calloc(50, sizeof(char));
    // Allocate memory for the password
    printf("username=");
    fgets(usr, 50, stdin);
    // Read the username from the console
    printf("password=");
    fgets(passwd, 50, stdin);
	usr[strcspn(usr, "\n")] = '\0';
    passwd[strcspn(passwd, "\n")] = '\0';
    // Read the password from the console
    JSON_Value *value = json_value_init_object();
    // Create a JSON object
    JSON_Object *object = json_value_get_object(value);
    // Get the JSON object from the root value
    const char *user = "username";
    // Define the key for the username field
    const char *password = "password";
    // Define the key for the password field
    json_object_set_string(object, user, usr);
    // Set the username field in the JSON object
    json_object_set_string(object, password, passwd);
    // Set the password field in the JSON object
    char *jsonString = json_serialize_to_string(value);
    // Serialize the JSON object to a string
    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
    // Open a connection
    message = compute_post_request("34.254.242.81", "/api/v1/tema/auth/register",
    "application/json", &jsonString, 1, NULL, 0, NULL);
    // Create the POST request message
    send_to_server(sockfd, message);  // Send the message to the server
    response = receive_from_server(sockfd);
    // Receive the response from the server
    if(strstr(response, "The username ") != NULL) {
        puts("208 - Reported.");
        // If the response contains "The username "
        // it means the username already exists
    } else if(strstr(response, "201 Created") != NULL) {
        puts("200 - OK.");
        // If the response contains "201 Created",
        // it means the registration was successful
    }
    free(usr);  // Free the memory allocated for the username
    free(passwd);  // Free the memory allocated for the password
    close_connection(sockfd);  // Close the connection
}

void login_handler(char *message, char *response, int sockfd, int *in,
char ***cookie) {
    char *usr = (char *)calloc(50, sizeof(char));
    // Allocate memory for the username
    char *passwd = (char *)calloc(50, sizeof(char));
    // Allocate memory for the password
    printf("username=");
    fgets(usr, 50, stdin);
    // Read the username from the console
    printf("password=");
    fgets(passwd, 50, stdin);
	usr[strcspn(usr, "\n")] = '\0';
    passwd[strcspn(passwd, "\n")] = '\0';
    // Read the password from the console
    JSON_Value *value = json_value_init_object();
    // Initialize a JSON object
    JSON_Object *object = json_value_get_object(value);
    // Get the JSON object from the value
    const char *user = "username";
    const char *password = "password";
    json_object_set_string(object, user, usr);
    // Set the username in the JSON object
    json_object_set_string(object, password, passwd);
    // Set the password in the JSON object
    char *jsonString = json_serialize_to_string(value);
    // Convert the JSON object to a string
    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
    // Open a connection
    message = compute_post_request("34.254.242.81", "/api/v1/tema/auth/login",
    "application/json", &jsonString, 1, NULL, 0, NULL);
    // Create the POST request message
    send_to_server(sockfd, message);
    // Send the request to the server
    response = receive_from_server(sockfd);
    // Receive the response from the server
    if (strstr(response, "200 OK") != NULL) {
        puts("200 - OK.");
        // Print a success message if the response contains "200 OK"
        *in = 1;
        // Set the 'in' flag to 1 to indicate successful login
    } else {
        puts("401 - Unauthorized.");
        // Print an error message if the response contains "401 Unauthorized"
    }
    char *start = strstr(response, "Set-Cookie: ");
    // Find the starting position of the "Set-Cookie" header
    if (start != NULL) {
        start = start + strlen("Set-Cookie: ");
        // Move the pointer past the "Set-Cookie: " prefix
        char *end = strchr(start, ';');
        // Find the position of the first semicolon (;)
        if (end != NULL) {
            size_t length = end - start;
            // Calculate the length of the token
            (*cookie)[0] = (char *)malloc((length + 1) * sizeof(char));
            // Allocate memory for the cookie token
            strncpy((*cookie)[0], start, length);
            // Copy the token to the cookie array
            (*cookie)[0][length] = '\0';
            // Add the null terminator at the end
        }
    } else {
        *cookie = NULL;
        // Set the cookie to NULL if no "Set-Cookie" header is found
    }
    free(usr);  // Free the memory allocated for the username
    free(passwd);  // Free the memory allocated for the password
    close_connection(sockfd);  // Close the connection
}

void enter_handler(char* message, char* response, int sockfd, int in, int *ok,
char** cookie, char** jwt) {
    if (!in || jwt == NULL) {
        puts("401 - Unauthorized.");
        // Check if user is not logged in or jwt token is not available
        return;
    }
    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
    // Open a connection to the server
    message = compute_get_request("34.254.242.81", "/api/v1/tema/library/access",
    NULL, cookie, 1, NULL);  // Create a GET request message
    send_to_server(sockfd, message);  // Send the message to the server
    response = receive_from_server(sockfd);
    // Receive the response from the server
    char *start = strstr(response, "token");
    // Find the starting position of the "token" string in the response
    if (start != NULL) {
        start = start + strlen("token") + 3;
        // Move the start pointer to the beginning of the actual token value
        char *stop = strchr(start, '\"');
        // Find the end position of the token value
        if (stop != NULL) {
            size_t length = stop - start;
            // Calculate the length of the token value
            *jwt = (char*)malloc(length * sizeof(char));
            // Allocate memory for the jwt token
            strcpy(*jwt, ""); // Initialize the jwt token as an empty string
            strncat(*jwt, start, length);
            // Copy the token value to the jwt token variable
            (*jwt)[length] = '\0';
            // Add a null terminator at the end of the jwt token
            *ok = 1;  // Set the flag indicating successful token extraction
        }
    }
    printf("token=%s\n", *jwt);  // Print the extracted jwt token
    close_connection(sockfd);  // Close the connection to the server
}

void get1_handler(char *message, char *response, int sockfd, int in, int ok,
char *jwt, char **cookie) {
    if (!in || jwt == NULL || ok == 0) {
        puts("401 - Unauthorized.");
        // Print error message if unauthorized or missing JWT
        return; // Exit the function
    }
    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
    // Open a connection
    message = compute_get_request("34.254.242.81", "/api/v1/tema/library/books",
    NULL, cookie, 1, jwt); // Create GET request message
    send_to_server(sockfd, message);  // Send the request to the server
    response = receive_from_server(sockfd);
    // Receive the response from the server
    const char* target = "[{";
    // Define the target string to search in the response
    char* tofind = get_last_line(response, target);
    // Get the last line containing the target string
    JSON_Value* root = json_parse_string(tofind);
    // Parse the last line as a JSON value
    if (root != NULL) {
        JSON_Array* array = json_value_get_array(root);
        // Get the JSON array from the parsed value
        if (array != NULL) {
            print_fields(array);
            // Print the fields of the JSON array
        }
        json_value_free(root);
        // Free the memory allocated for the JSON value
    }
    close_connection(sockfd);
    // Close the connection
}

void get2_handler(char *message, char *response, int sockfd, int in, int ok,
char *jwt, char **cookie) {
    if (!in || jwt == NULL || ok == 0) {
        puts("401 - Unauthorized.");  // Unauthorized condition check
        return;
    }
    int id = -1; // Initialize the id variable
    char char_id[MAX_LENGTH];  // Create a character array to store user input
    printf("id=");
    fgets(char_id, MAX_LENGTH, stdin);  // Read user input for id
    id = atoi(char_id);  // Convert the input to an integer
    char url[MAX_LENGTH + 28];  // Create a URL string with a maximum length
    sprintf(url, "/api/v1/tema/library/books/%d", id);
    // Construct the URL with the given id
    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
    // Open a connection
    message = compute_get_request("34.254.242.81", url, NULL, cookie, 1, jwt);
    // Create the GET request message
    send_to_server(sockfd, message);  // Send the message to the server
    response = receive_from_server(sockfd);
    // Receive the response from the server
    const char* target = "{";
    // Set the target delimiter for parsing the JSON object
    char* tofind = get_last_line(response, target);
    // Get the last line containing the JSON object
    JSON_Value* value = json_parse_string(tofind);
    // Parse the JSON object
    if (value != NULL) {
        JSON_Object* object = json_value_get_object(value);
        // Get the JSON object
        printf("title: %s\n", json_object_get_string(object, "title"));
        // Print the title field
        printf("author: %s\n", json_object_get_string(object, "author"));
        // Print the author field
        printf("publisher: %s\n", json_object_get_string(object, "publisher"));
        // Print the publisher field
        printf("genre: %s\n", json_object_get_string(object, "genre"));
        // Print the genre field
        printf("page_count: %d\n", (int)json_object_get_number(object, "page_count"));
        // Print the page_count field
        json_value_free(value);  // Free the JSON value
    } else {
        printf("404 - Not Found.\n");  // Not found condition check
    }
    close_connection(sockfd);  // Close the connection
}

void add_handler(char *message, char *response, int sockfd, int in, int ok,
char *jwt, char **cookie) {
    if (!in || jwt == NULL || ok == 0) {
        puts("401 - Unauthorized.");
        // Unauthorized condition, return with an error message
        return;
    }
    JSON_Value *value = json_value_init_object();
    JSON_Object *object = json_value_get_object(value);
    char bufferul[100];
    printf("title=");  // Prompt for title input
    fgets(bufferul, 100, stdin);  // Read user input for title
    bufferul[strcspn(bufferul, "\n")] = '\0';
    // Remove newline character from input
    if (strlen(bufferul) == 0 || strpbrk(bufferul,
    "0123456789/;<>|][{}]") != NULL) {
        printf("400 - Bad Request.\n");
        // Invalid input characters, return with an error message
        return;
    } else {
        json_object_set_string(object, "title", bufferul);
        // Set the title in the JSON object
    }
    // Repeat the above process for author, publisher, and genre inputs
    printf("author=");  // Prompt for author input
    fgets(bufferul, 100, stdin);  // Read user input for author
    bufferul[strcspn(bufferul, "\n")] = '\0';
    // Remove newline character from input
    if (strlen(bufferul) == 0 || strpbrk(bufferul,
    "0123456789/;<>|][{}]") != NULL) {
        printf("400 - Bad Request.\n");
        // Invalid input characters, return with an error message
        return;
    } else {
        json_object_set_string(object, "author", bufferul);
        // Set the author in the JSON object
    }
    printf("publisher=");  // Prompt for publisher input
    fgets(bufferul, 100, stdin);  // Read user input for publisher
    bufferul[strcspn(bufferul, "\n")] = '\0';
    // Remove newline character from input
    if (strlen(bufferul) == 0 || strpbrk(bufferul,
    "0123456789/;<>|][{}]") != NULL) {
        printf("400 - Bad Request.\n");
        // Invalid input characters, return with an error message
        return;
    } else {
        json_object_set_string(object, "publisher", bufferul);
        // Set the publisher in the JSON object
    }
    printf("genre=");  // Prompt for genre input
    fgets(bufferul, 100, stdin);  // Read user input for genre
    bufferul[strcspn(bufferul, "\n")] = '\0';
    // Remove newline character from input
    if (strlen(bufferul) == 0 || strpbrk(bufferul,
    "0123456789/;<>|][{}]") != NULL) {
        printf("400 - Bad Request.\n");
        // Invalid input characters, return with an error message
        return;
    } else {
        json_object_set_string(object, "genre", bufferul);
        // Set the genre in the JSON object
    }
    char pg_str[10];
    int pg;
    printf("page_count=");
    if (fgets(pg_str, sizeof(pg_str), stdin) != NULL) {
        if (sscanf(pg_str, "%d", &pg) != 1) {
            printf("400 - Bad Request.\n");
            // Invalid page count, return with an error message
            return;
        }
    } else {
        printf("400 - Bad Request.\n");
        // Invalid input, return with an error message
        return;
    }
    json_object_set_number(object, "page_count", pg);
    // Set the page count in the JSON object
    char *jsonString = json_serialize_to_string(value);
    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
    // Open connection to server
    message = compute_post_request("34.254.242.81",
    "/api/v1/tema/library/books", "application/json", &jsonString, 1,
    cookie, 1, jwt);  // Create the POST request message
    send_to_server(sockfd, message);  // Send the request to the server
    response = receive_from_server(sockfd);  // Receive the response from the server
    puts("201 - Created.");  // Successful creation response
    json_free_serialized_string(jsonString);
    json_value_free(value);
    close_connection(sockfd);  // Close the connection
}

void delete_handler(char *message, char *response, int sockfd, int in, int ok,
char *jwt, char **cookie) {
    // Check authorization and validity conditions
    if (!in || jwt == NULL || ok == 0) {
        puts("401 - Unauthorized.");
        return;
    }
    int id = -1;
    char char_id[MAX_LENGTH];
    // Read the ID input from the user
    printf("id=");
    fgets(char_id, MAX_LENGTH, stdin);
    // Convert the ID from string to integer
    id = atoi(char_id);
    char url[MAX_LENGTH + 28];
    // Create the URL for the delete request
    sprintf(url, "/api/v1/tema/library/books/%d", id);
    // Open a connection to the server
    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
    // Construct and send the delete request
    message = compute_delete_request("34.254.242.81", url, "application/json",
    NULL, 0, cookie, 1, jwt);
    send_to_server(sockfd, message);
    // Receive the response from the server
    response = receive_from_server(sockfd);
    puts("204 - No Content.");
    // Close the connection
    close_connection(sockfd);
}

void logout_handler(char *message, char *response, int sockfd, int *in, int *ok,
char *jwt, char **cookie) {
    if (!(*in) || *cookie == NULL) {
        puts("401 - Unauthorized.");
        // Print unauthorized message if not logged in or cookie is missing
        return;
    }
    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
    // Open connection to server
    message = compute_get_request("34.254.242.81",
    "/api/v1/tema/auth/logout", NULL, cookie, 1, NULL);
    // Create logout request message
    send_to_server(sockfd, message);
    // Send logout request to the server
    response = receive_from_server(sockfd);
    // Receive response from the server
    if (strstr(response, "200 OK") != NULL) {
        puts("204 - No Content.");
        // Print success message if logout is successful
        *in = 0;  // Set 'in' flag to indicate user is logged out
        *ok = 0;  // Set 'ok' flag to indicate logout success
        free(*cookie);  // Free the memory allocated for the cookie
        *cookie = NULL;  // Set the cookie pointer to NULL
    } else {
        puts("400 - Bad Request.");
        // Print error message if logout request is invalid
    }
    close_connection(sockfd);
    // Close the connection to the server
}

int main(int argc, char *argv[]) {
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    // Allocate memory for the message
    char *response = (char*)calloc(BUFLEN, sizeof(char));
    // Allocate memory for the response
    int sockfd = 0; // Socket file descriptor
    int in = 0; // Variable to track login status
    char *jwt = (char*)calloc(BUFLEN, sizeof(char));
    // JWT token
    char **cookie = (char**)calloc(10, sizeof(char*));
    // Array to store cookie elements
    char *cmd = (char*)calloc(50, sizeof(char));  // User command input
    int ok = 0;  // Variable to track authorization status
    while (1) {
        printf("command=");  // Prompt for user command input
        fgets(cmd, BUFLEN, stdin);  // Read user command input from stdin
        remove_whitespace(cmd);
        // Remove whitespace, tabs, and newlines from the input
        // Determine the command code and
        // execute the corresponding handler function
        switch (code(cmd)) {
            case 0:
                exit_handler();  // Exit the program
                break;
            case 1:
                register_handler(message, response, sockfd);
                // Perform user registration
                break;
            case 2:
                login_handler(message, response, sockfd, &in, &cookie);
                // Perform user login
                break;
            case 3:
                enter_handler(message, response, sockfd, in, &ok, cookie, &jwt);
                // Enter the library
                break;
            case 4:
                get1_handler(message, response, sockfd, in, ok, jwt, cookie);
                // Get all books
                break;
            case 5:
                get2_handler(message, response, sockfd, in, ok, jwt, cookie);
                // Get a specific book
                break;
            case 6:
                add_handler(message, response, sockfd, in, ok, jwt, cookie);
                // Add a book
                break;
            case 7:
                delete_handler(message, response, sockfd, in, ok, jwt, cookie);
                // Delete a book
                break;
            case 8:
                logout_handler(message, response, sockfd, &in, &ok, jwt, cookie);
                // Logout the user
                break;
            default:
                puts("400 - Bad request.");
                // Invalid command
                break;
        }
    }
    // Free memory for each cookie element
    for (int i = 0; i < 10; i++) {
        if (cookie[i] != NULL) {
            puts(cookie[i]);
            free(cookie[i]);
        }
    }
    free(jwt);  // Free memory for JWT token
    free(message);  // Free memory for message
    free(response);  // Free memory for response
    free(cookie);  // Free memory for cookie array
    free(cmd);  // Free memory for user command input
    return 0;
}
