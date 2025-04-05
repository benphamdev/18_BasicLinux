#include "web_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

// Buffer for capturing demo output
static char output_buffer[65536];
static pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

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
    
    static int request_marker;
    struct MHD_Response* response;
    int ret;
    
    // Set up a request marker for the first call
    if (*con_cls == NULL) {
        *con_cls = &request_marker;
        return MHD_YES;
    }
    
    if (strcmp(method, "GET") != 0) {
        // Only handle GET requests
        return MHD_NO;
    }
    
    printf("Received request for URL: %s\n", url);
    
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