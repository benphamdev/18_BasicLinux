#include "../../include/web_server.h"
#include "../../include/ai_integration.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>

// Structure to store POST request data
struct PostConnectionData {
    char *data;
    size_t size;
    int is_first_call;
};

// Forward declarations to fix implicit declaration errors
char* extract_json_value(const char* json, const char* key);
char* create_json_response(const char* message);
char* process_ai_request(const char* message);
char* generate_project_context(void);
void scan_directory(const char *path, char *buffer, size_t *pos, size_t *buffer_size, int depth, int max_depth);
void add_file_content(const char *file_path, char *buffer, size_t *pos, size_t *buffer_size, int max_lines);
char* load_template(const char* filename);
char* generate_demo_html(void);
char* capture_demo_output(void (*demo_func)(void));
const char* get_content_type(const char* filename);

// Buffer for capturing demo output
static char output_buffer[65536];
static pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

// Global project context for DeepSeek AI
static char *project_context = NULL;
static pthread_mutex_t context_mutex = PTHREAD_MUTEX_INITIALIZER;

// Demo function declarations - from your existing code
extern void demo_file_operations();
extern void demo_process_operations();
extern void demo_directory_operations();
extern void demo_pipe_operations();
extern void demo_time_operations();
extern void demo_signal_operations();
extern void demo_memory_operations();
extern void demo_file_permission_operations();
extern void demo_system_info_operations();
extern void demo_user_group_operations();
extern void demo_filesystem_operations();
extern void demo_terminal_operations();
extern void demo_message_queue_operations();
extern void demo_event_monitoring();
extern void demo_file_monitoring();
extern void demo_process_scheduling();
extern void demo_advanced_file_operations();
extern void demo_extended_attributes();
extern void demo_advanced_memory_management();
extern void demo_advanced_networking();
extern void demo_system_timers();
extern void demo_process_priority();
extern void demo_user_group_ids();
extern void demo_file_locking();

// List of all available demos
static const demo_info_t demos[] = {
    {"file_operations", "File Operations", demo_file_operations},
    {"process_operations", "Process Operations", demo_process_operations},
    {"directory_operations", "Directory Operations", demo_directory_operations},
    {"pipe_operations", "Pipe Operations", demo_pipe_operations},
    {"time_operations", "Time Operations", demo_time_operations},
    {"signal_operations", "Signal Handling", demo_signal_operations},
    {"memory_operations", "Memory Management", demo_memory_operations},
    {"file_permission_operations", "File Permission Management", demo_file_permission_operations},
    {"system_info_operations", "System Information", demo_system_info_operations},
    {"user_group_operations", "User and Group Management", demo_user_group_operations},
    {"filesystem_operations", "Filesystem Operations", demo_filesystem_operations},
    {"terminal_operations", "Terminal I/O Operations", demo_terminal_operations},
    {"message_queue_operations", "Message Queue Operations", demo_message_queue_operations},
    {"event_monitoring", "Event Monitoring", demo_event_monitoring},
    {"file_monitoring", "File Monitoring", demo_file_monitoring},
    {"process_scheduling", "Process Scheduling", demo_process_scheduling},
    {"advanced_file_operations", "Advanced File Operations", demo_advanced_file_operations},
    {"extended_attributes", "Extended Attributes", demo_extended_attributes},
    {"advanced_memory_management", "Advanced Memory Management", demo_advanced_memory_management},
    {"advanced_networking", "Advanced Networking", demo_advanced_networking},
    {"system_timers", "System Timers", demo_system_timers},
    {"process_priority", "Process Priority & Resource Usage", demo_process_priority},
    {"user_group_ids", "User and Group IDs", demo_user_group_ids},
    {"file_locking", "File Locking", demo_file_locking},
    {NULL, NULL, NULL} // Terminator
};

// Initialize the web server
struct MHD_Daemon* init_web_server(void) {
    // Initialize the AI system first
    // Using the from_env_file version which doesn't need an explicit API key
    // It will read from .env file or environment variables
    if (!ai_init_from_env_file(NULL)) {
        fprintf(stderr, "Failed to initialize AI system\n");
        // Continue anyway, other functionality will still work
    } else {
        printf("AI system initialized successfully\n");
    }

    struct MHD_Daemon* daemon = MHD_start_daemon(
        MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG,
        SERVER_PORT,
        NULL, NULL,
        &handle_request, NULL,
        MHD_OPTION_END);
    
    if (daemon == NULL) {
        fprintf(stderr, "Failed to start web server\n");
    } else {
        printf("Web server started at http://localhost:%d\n", SERVER_PORT);
    }
    
    return daemon;
}

// Stop the web server
void stop_web_server(struct MHD_Daemon* daemon) {
    if (daemon != NULL) {
        MHD_stop_daemon(daemon);
        printf("Web server stopped\n");
    }
}

// Handle HTTP requests
int handle_request(void* cls, struct MHD_Connection* connection,
                 const char* url, const char* method, const char* version,
                 const char* upload_data, size_t* upload_data_size, void** con_cls) {
    
    struct MHD_Response* response;
    int ret;
    
    // First call setup
    if (*con_cls == NULL) {
        // For POST requests, allocate a structure to store data
        if (strcmp(method, "POST") == 0) {
            struct PostConnectionData *post_data = malloc(sizeof(struct PostConnectionData));
            if (post_data == NULL) {
                return MHD_NO;
            }
            post_data->data = NULL;
            post_data->size = 0;
            post_data->is_first_call = 1;
            *con_cls = post_data;
        } else {
            // For non-POST requests, just use a dummy marker
            static int dummy;
            *con_cls = &dummy;
        }
        return MHD_YES;
    }

    printf("Received request: %s %s\n", method, url);
    
    // Handle POST request for chat API
    if (strcmp(url, "/api/chat") == 0 && strcmp(method, "POST") == 0) {
        struct PostConnectionData *post_data = *con_cls;
        
        // Process incoming data chunks
        if (*upload_data_size != 0) {
            // Append the new chunk to our buffer
            char *new_data = realloc(post_data->data, post_data->size + *upload_data_size + 1);
            if (new_data == NULL) {
                // Memory allocation error
                if (post_data->data) {
                    free(post_data->data);
                }
                free(post_data);
                *con_cls = NULL;
                return MHD_NO;
            }
            
            post_data->data = new_data;
            memcpy(post_data->data + post_data->size, upload_data, *upload_data_size);
            post_data->size += *upload_data_size;
            post_data->data[post_data->size] = '\0';
            
            // Mark that we've consumed this chunk
            *upload_data_size = 0;
            
            // Wait for more data
            return MHD_YES;
        }
        
        // No more data - process the complete POST request
        if (post_data->data != NULL) {
            const char *content_type = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Content-Type");
            
            printf("Received POST data: %s\n", post_data->data);
            
            if (content_type != NULL && strstr(content_type, "application/json") != NULL) {
                // Process the JSON data
                char *message = extract_json_value(post_data->data, "message");
                
                if (message != NULL) {
                    printf("Extracted message: %s\n", message);
                    
                    // Call DeepSeek API
                    char *ai_response = process_ai_request(message);
                    free(message);
                    
                    // Create JSON response
                    char *json_response = create_json_response(ai_response ? ai_response : "Error processing request");
                    if (ai_response) {
                        free(ai_response);
                    }
                    
                    // Send response
                    response = MHD_create_response_from_buffer(
                        strlen(json_response),
                        json_response,
                        MHD_RESPMEM_MUST_FREE
                    );
                    MHD_add_response_header(response, "Content-Type", "application/json");
                    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
                    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
                    MHD_destroy_response(response);
                    
                    // Clean up
                    if (post_data->data) free(post_data->data);
                    free(post_data);
                    *con_cls = NULL;
                    
                    return ret;
                }
            }
            
            // If we get here, something went wrong with processing the data
            const char *error_json = "{\"error\":\"Could not parse request data\"}";
            response = MHD_create_response_from_buffer(
                strlen(error_json),
                (void*)error_json,
                MHD_RESPMEM_PERSISTENT
            );
            MHD_add_response_header(response, "Content-Type", "application/json");
            MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
            ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
            MHD_destroy_response(response);
            
            // Clean up
            if (post_data->data) free(post_data->data);
            free(post_data);
            *con_cls = NULL;
            
            return ret;
        }
    }
    
    // Handle OPTIONS request for CORS preflight
    if (strcmp(method, "OPTIONS") == 0) {
        response = MHD_create_response_from_buffer(0, "", MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
        MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");
        MHD_add_response_header(response, "Access-Control-Max-Age", "86400");
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }
    
    // Handle GET request for project context
    if (strcmp(url, "/api/project-context") == 0 && strcmp(method, "GET") == 0) {
        char *json_response;
        
        // Generate project context if we don't have it yet
        pthread_mutex_lock(&context_mutex);
        if (project_context == NULL) {
            project_context = generate_project_context();
        }
        
        if (project_context != NULL) {
            size_t response_len = strlen(project_context) + 50;
            json_response = malloc(response_len);
            if (json_response != NULL) {
                snprintf(json_response, response_len, "{\"success\":true,\"contextSize\":%zu}", strlen(project_context));
            } else {
                json_response = strdup("{\"success\":false,\"error\":\"Memory allocation failed\"}");
            }
        } else {
            json_response = strdup("{\"success\":false,\"error\":\"Failed to generate project context\"}");
        }
        pthread_mutex_unlock(&context_mutex);
        
        response = MHD_create_response_from_buffer(
            strlen(json_response),
            json_response,
            MHD_RESPMEM_MUST_FREE
        );
        MHD_add_response_header(response, "Content-Type", "application/json");
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }
    
    // Handle DeepSeek chat page
    if (strcmp(url, "/deepseek-chat") == 0 || strcmp(url, "/deepseek-chat/") == 0) {
        char *chat_html = load_template("deepseek_chat.html");
        
        response = MHD_create_response_from_buffer(
            strlen(chat_html),
            chat_html,
            MHD_RESPMEM_MUST_FREE
        );
        MHD_add_response_header(response, "Content-Type", "text/html");
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }
    
    // Handle static files (CSS, JS, images)
    if (strstr(url, ".css") || strstr(url, ".js") || 
        strstr(url, ".png") || strstr(url, ".jpg") || strstr(url, ".ico")) {
        
        char file_path[512];
        snprintf(file_path, sizeof(file_path), "%s%s", STATIC_DIR, url + 1); // Skip leading /
        
        printf("Trying to serve file: %s\n", file_path);
        
        FILE* file = fopen(file_path, "rb");
        if (file == NULL) {
            printf("File not found: %s\n", file_path);
            const char* not_found = "<html><body><h1>404 Not Found</h1></body></html>";
            response = MHD_create_response_from_buffer(strlen(not_found),
                                                     (void*)not_found,
                                                     MHD_RESPMEM_PERSISTENT);
            ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
            MHD_destroy_response(response);
            return ret;
        }
        
        // Get file size
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        // Read the file
        char* buffer = malloc(file_size);
        if (buffer == NULL) {
            fclose(file);
            return MHD_NO;
        }
        
        fread(buffer, 1, file_size, file);
        fclose(file);
        
        // Create response
        printf("Serving file: %s, size: %ld bytes, Content-Type: %s\n", 
               file_path, file_size, get_content_type(url));
        response = MHD_create_response_from_buffer(file_size, buffer, MHD_RESPMEM_MUST_FREE);
        MHD_add_response_header(response, "Content-Type", get_content_type(url));
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }
    
    // Handle API endpoint for running a demo
    if (strncmp(url, "/run/", 5) == 0) {
        const char* demo_name = url + 5; // Skip "/run/"
        
        // Find the requested demo
        for (int i = 0; demos[i].name != NULL; i++) {
            if (strcmp(demo_name, demos[i].name) == 0) {
                printf("Running demo: %s\n", demo_name);
                
                // Run the demo and capture output
                char* output = capture_demo_output(demos[i].function);
                
                // Log output size
                size_t output_len = strlen(output);
                printf("Captured %zu bytes of output\n", output_len);
                
                // Create JSON response
                size_t json_size = output_len + 100; // Add extra space for JSON format
                char* json = malloc(json_size);
                
                if (json == NULL) {
                    free(output);
                    return MHD_NO;
                }
                
                snprintf(json, json_size, "{\"status\":\"success\",\"output\":\"%s\"}", output);
                free(output);
                
                response = MHD_create_response_from_buffer(strlen(json), json, MHD_RESPMEM_MUST_FREE);
                MHD_add_response_header(response, "Content-Type", "application/json");
                MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
                ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
                MHD_destroy_response(response);
                
                printf("Response sent\n");
                return ret;
            }
        }
        
        printf("Demo not found: %s\n", demo_name);
        
        // Demo not found
        const char* error = "{\"status\":\"error\",\"message\":\"Demo not found\"}";
        response = MHD_create_response_from_buffer(strlen(error),
                                                 (void*)error,
                                                 MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, "Content-Type", "application/json");
        ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
        MHD_destroy_response(response);
        return ret;
    }
    
    // Main page - generate HTML
    if (strcmp(url, "/") == 0 || strcmp(url, "/index.html") == 0) {
        printf("Generating main page HTML\n");
        char* page_content = generate_demo_html();
        
        response = MHD_create_response_from_buffer(strlen(page_content),
                                                (void*)page_content,
                                                MHD_RESPMEM_MUST_FREE);
        MHD_add_response_header(response, "Content-Type", "text/html");
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        
        return ret;
    }
    
    // Not found
    const char* not_found = "<html><body><h1>404 Not Found</h1></body></html>";
    response = MHD_create_response_from_buffer(strlen(not_found),
                                            (void*)not_found,
                                            MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    MHD_destroy_response(response);
    return ret;
}

// Extract JSON value from a key
char* extract_json_value(const char* json, const char* key) {
    // Simple JSON parser (for production, use a proper JSON library)
    char search_key[256];
    snprintf(search_key, sizeof(search_key), "\"%s\":\"", key);
    
    char* start = strstr(json, search_key);
    if (start == NULL) {
        return NULL;
    }
    
    start += strlen(search_key);
    char* end = strchr(start, '"');
    if (end == NULL) {
        return NULL;
    }
    
    size_t len = end - start;
    char* value = malloc(len + 1);
    if (value == NULL) {
        return NULL;
    }
    
    strncpy(value, start, len);
    value[len] = '\0';
    return value;
}

// Create a JSON response
char* create_json_response(const char* message) {
    // Escape special characters in message
    size_t message_len = strlen(message);
    size_t json_size = message_len * 2 + 100; // Allow extra space for escaping and JSON format
    
    char* json = malloc(json_size);
    if (json == NULL) {
        return NULL;
    }
    
    // Start with JSON opening
    strcpy(json, "{\"response\":\"");
    
    // Copy and escape message
    size_t pos = strlen(json);
    for (size_t i = 0; i < message_len; i++) {
        char c = message[i];
        if (c == '"' || c == '\\' || c == '\n' || c == '\r') {
            json[pos++] = '\\';
            if (c == '\n') {
                json[pos++] = 'n';
            } else if (c == '\r') {
                json[pos++] = 'r';
            } else {
                json[pos++] = c;
            }
        } else {
            json[pos++] = c;
        }
        
        // Check if we're about to overflow
        if (pos >= json_size - 10) {
            json_size *= 2;
            char* new_json = realloc(json, json_size);
            if (new_json == NULL) {
                free(json);
                return NULL;
            }
            json = new_json;
        }
    }
    
    // Add JSON closing
    strcpy(json + pos, "\"}");
    
    return json;
}

// Process an AI request using DeepSeek
char* process_ai_request(const char* message) {
    // Check if we have project context
    pthread_mutex_lock(&context_mutex);
    char *prompt;
    
    if (project_context != NULL) {
        // Combine project context with user message
        size_t prompt_size = strlen(message) + strlen(project_context) + 200;
        prompt = malloc(prompt_size);
        if (prompt != NULL) {
            snprintf(prompt, prompt_size, 
                "You are an AI assistant helping with a C programming project. "
                "Here's the context about the project structure:\n\n%s\n\n"
                "User question: %s", 
                project_context, message);
        } else {
            prompt = strdup(message);
        }
    } else {
        prompt = strdup(message);
    }
    pthread_mutex_unlock(&context_mutex);
    
    if (prompt == NULL) {
        return strdup("Error creating prompt");
    }
    
    // Call DeepSeek API
    char *response = ai_generate_text(prompt, NULL);
    
    // Check if the AI failed and try to reinitialize
    if (response == NULL || strstr(response, "AI not initialized") != NULL) {
        // Attempt to reinitialize the AI
        printf("AI appears to be uninitialized. Attempting to reinitialize...\n");
        
        // Free existing error response if any
        if (response) {
            free(response);
            response = NULL;
        }
        
        // Try to initialize the AI
        if (ai_init_from_env_file(NULL)) {
            printf("AI successfully reinitialized. Retrying request...\n");
            // Retry the request
            response = ai_generate_text(prompt, NULL);
        } else {
            printf("AI reinitialization failed\n");
            response = strdup("The AI system could not be initialized. Please check your API key configuration.");
        }
    }
    
    free(prompt);
    
    // If still null, provide a fallback response
    if (response == NULL) {
        return strdup("Error processing request. The AI service might be unavailable.");
    }
    
    return response;
}

// Scan project directory and generate context
char* generate_project_context() {
    // Buffer to store project structure
    size_t buffer_size = 65536;  // Start with 64KB
    char *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        return NULL;
    }
    
    size_t pos = 0;
    pos += snprintf(buffer + pos, buffer_size - pos, 
        "Project Structure Overview:\n\n");
    
    // Start scanning from the project root
    scan_directory(".", buffer, &pos, &buffer_size, 0, 5);  // Max depth of 5
    
    // Add some key file contents
    pos += snprintf(buffer + pos, buffer_size - pos, 
        "\n\nKey Files Summary:\n\n");
    
    // Add main.c
    add_file_content("src/main.c", buffer, &pos, &buffer_size, 100);  // First 100 lines
    
    // Add syscalls.h
    add_file_content("include/syscalls.h", buffer, &pos, &buffer_size, 50);  // First 50 lines
    
    // Add ai_integration.h
    add_file_content("include/ai_integration.h", buffer, &pos, &buffer_size, 100);  // All lines
    
    // Ensure buffer is null-terminated
    if (pos < buffer_size) {
        buffer[pos] = '\0';
    } else {
        buffer[buffer_size - 1] = '\0';
    }
    
    return buffer;
}

// Recursively scan directory and add to context
void scan_directory(const char *path, char *buffer, size_t *pos, size_t *buffer_size, int depth, int max_depth) {
    if (depth > max_depth) {
        return;
    }
    
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    
    // Add indentation based on depth
    char indent[32] = {0};
    for (int i = 0; i < depth; i++) {
        strcat(indent, "  ");
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip hidden files and special directories
        if (entry->d_name[0] == '.') {
            continue;
        }
        
        // Check if we need to resize buffer
        if (*pos + 256 > *buffer_size) {
            *buffer_size *= 2;
            char *new_buffer = realloc(buffer, *buffer_size);
            if (new_buffer == NULL) {
                closedir(dir);
                return;
            }
            buffer = new_buffer;
        }
        
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
        struct stat st;
        if (stat(full_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                // Directory
                *pos += snprintf(buffer + *pos, *buffer_size - *pos, 
                    "%s- %s/\n", indent, entry->d_name);
                
                // Recursively scan subdirectory
                scan_directory(full_path, buffer, pos, buffer_size, depth + 1, max_depth);
            } else {
                // File
                *pos += snprintf(buffer + *pos, *buffer_size - *pos, 
                    "%s- %s\n", indent, entry->d_name);
            }
        }
    }
    
    closedir(dir);
}

// Add file content to context buffer
void add_file_content(const char *file_path, char *buffer, size_t *pos, size_t *buffer_size, int max_lines) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        *pos += snprintf(buffer + *pos, *buffer_size - *pos, 
            "Cannot open file: %s\n", file_path);
        return;
    }
    
    // Add file header
    *pos += snprintf(buffer + *pos, *buffer_size - *pos, 
        "File: %s\n```c\n", file_path);
    
    char line[1024];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), file) && line_count < max_lines) {
        size_t line_len = strlen(line);
        
        // Check if we need to resize buffer
        if (*pos + line_len + 10 > *buffer_size) {
            *buffer_size *= 2;
            char *new_buffer = realloc(buffer, *buffer_size);
            if (new_buffer == NULL) {
                fclose(file);
                return;
            }
            buffer = new_buffer;
        }
        
        // Add line to buffer
        memcpy(buffer + *pos, line, line_len);
        *pos += line_len;
        line_count++;
    }
    
    if (!feof(file)) {
        *pos += snprintf(buffer + *pos, *buffer_size - *pos, 
            "... (truncated, %d lines shown)\n", max_lines);
    }
    
    // Add file footer
    *pos += snprintf(buffer + *pos, *buffer_size - *pos, "```\n\n");
    
    fclose(file);
}

// Load a template file
char* load_template(const char* filename) {
    char path[512];
    snprintf(path, sizeof(path), "%s%s", TEMPLATE_DIR, filename);
    
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open template file: %s\n", path);
        return strdup("<!-- Template not found -->");
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Read the file
    char* buffer = malloc(file_size + 1);
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    
    fclose(file);
    return buffer;
}

// Generate HTML for the demo list
char* generate_demo_html() {
    char* header = load_template("header.html");
    char* footer = load_template("footer.html");
    
    // Estimate size for the HTML
    size_t html_size = strlen(header) + strlen(footer) + 10000; // Allow extra space for the demos
    char* html = malloc(html_size);
    if (!html) {
        free(header);
        free(footer);
        return strdup("<html><body><h1>Memory allocation error</h1></body></html>");
    }
    
    // Start with the header
    strcpy(html, header);
    free(header);
    
    // Generate demo list
    strcat(html, "<div class='demo-list'>");
    for (int i = 0; demos[i].name != NULL; i++) {
        char demo_html[512];
        snprintf(demo_html, sizeof(demo_html), 
                "<div class='demo-item' data-demo='%s'>\n"
                "  <h3>%s</h3>\n"
                "  <button class='run-button'>Run Demo</button>\n"
                "</div>\n",
                demos[i].name, demos[i].description);
        strcat(html, demo_html);
    }
    strcat(html, "</div>");
    
    // Output area
    strcat(html, "<div class='output-container'>\n"
                "<h2>Demo Output</h2>\n"
                "<pre id='output'>Select a demo to run...</pre>\n"
                "</div>");
    
    // Add footer
    strcat(html, footer);
    free(footer);
    
    return html;
}

// Function to run a demo and capture its output
char* capture_demo_output(void (*demo_func)()) {
    // Create a pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return strdup("Error creating pipe for output capture");
    }
    
    // Fork a child process
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return strdup("Error forking process for demo");
    }
    
    if (pid == 0) {
        // Child process - redirect stdout and stderr to the pipe
        close(pipefd[0]); // Close read end
        
        if (dup2(pipefd[1], STDOUT_FILENO) == -1 ||
            dup2(pipefd[1], STDERR_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        
        close(pipefd[1]); // Close original write end
        
        // Run the demo
        demo_func();
        fflush(stdout); // Make sure all output is flushed
        
        // Exit child
        exit(EXIT_SUCCESS);
    } else {
        // Parent process - read from the pipe
        close(pipefd[1]); // Close write end
        
        pthread_mutex_lock(&output_mutex);
        
        // Initialize output buffer
        memset(output_buffer, 0, sizeof(output_buffer));
        
        // Read the output with a timeout (to ensure we get everything)
        fd_set readfds;
        struct timeval tv;
        int totalBytesRead = 0;
        
        while (1) {
            FD_ZERO(&readfds);
            FD_SET(pipefd[0], &readfds);
            
            // Set timeout to 1 second
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            
            int ret = select(pipefd[0] + 1, &readfds, NULL, NULL, &tv);
            if (ret == -1) {
                break; // Error
            } else if (ret == 0) {
                // Timeout - no more data
                break;
            }
            
            // Read available data
            ssize_t bytesRead = read(pipefd[0], 
                                    output_buffer + totalBytesRead, 
                                    sizeof(output_buffer) - totalBytesRead - 1);
            if (bytesRead <= 0) {
                break; // EOF or error
            }
            
            totalBytesRead += bytesRead;
            
            // Check if buffer is full
            if (totalBytesRead >= sizeof(output_buffer) - 1) {
                break;
            }
        }
        
        // Ensure null termination
        output_buffer[totalBytesRead] = '\0';
        
        // Wait for child to complete
        int status;
        waitpid(pid, &status, 0);
        
        // Process output for JSON
        char* json_output = malloc(totalBytesRead * 2 + 1); // Allocate twice the size for escaping
        if (json_output) {
            int j = 0;
            for (int i = 0; i < totalBytesRead; i++) {
                char c = output_buffer[i];
                if (c == '"') {
                    json_output[j++] = '\\';
                    json_output[j++] = '"';
                } else if (c == '\\') {
                    json_output[j++] = '\\';
                    json_output[j++] = '\\';
                } else if (c == '\n') {
                    json_output[j++] = '\\';
                    json_output[j++] = 'n';
                } else if (c == '\t') {
                    json_output[j++] = '\\';
                    json_output[j++] = 't';
                } else if (c == '\r') {
                    // Skip carriage returns
                } else if (c >= 32 && c <= 126) { // Printable ASCII
                    json_output[j++] = c;
                } else {
                    // For non-printable characters, use a space
                    json_output[j++] = ' ';
                }
            }
            json_output[j] = '\0';
        } else {
            // Memory allocation failed
            json_output = strdup("Memory allocation error");
        }
        
        close(pipefd[0]); // Close read end
        pthread_mutex_unlock(&output_mutex);
        
        return json_output;
    }
}

// Determine content type based on file extension
const char* get_content_type(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (ext == NULL) {
        return "text/plain";
    }
    
    if (strcmp(ext, ".html") == 0) return "text/html";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".js") == 0) return "application/javascript";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".gif") == 0) return "image/gif";
    if (strcmp(ext, ".ico") == 0) return "image/x-icon";
    
    return "text/plain";
}