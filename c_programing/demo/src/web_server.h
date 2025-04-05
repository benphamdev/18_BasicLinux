#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <microhttpd.h>
#include "../lib/syscalls.h"
#include "demos.h"

// Web server configuration
#define SERVER_PORT 8080
#define TEMPLATE_DIR "web/templates/"
#define STATIC_DIR "web/"

// Structure to hold demo information
typedef struct {
    const char* name;
    const char* description;
    void (*function)();
} demo_info_t;

// Initialize the web server
struct MHD_Daemon* init_web_server(void);

// Stop the web server
void stop_web_server(struct MHD_Daemon* daemon);

// Handle HTTP requests
int handle_request(void* cls, 
                  struct MHD_Connection* connection,
                  const char* url,
                  const char* method, 
                  const char* version,
                  const char* upload_data,
                  size_t* upload_data_size, 
                  void** con_cls);

// Utility functions
char* load_template(const char* filename);
char* generate_demo_html();
char* capture_demo_output(void (*demo_func)());
const char* get_content_type(const char* filename);

#endif // WEB_SERVER_H