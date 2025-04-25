#include "../include/ai_integration.h"
#include "../include/syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

// DeepSeek API endpoint
#define DEEPSEEK_API_URL "https://api.deepseek.com/v1/chat/completions"
// Default model if none specified
#define DEFAULT_MODEL "deepseek-chat"
// API key environment variable name to look for in .env file
#define API_KEY_ENV_VAR "DEEPSEEK_API_KEY="

// Static variables
static char *api_key = NULL;
static float temperature = 0.7f;

// Structure to store response data
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback function for curl to handle received data
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }
 
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
 
    return realsize;
}

/**
 * Read API key from environment file (format: KEY=VALUE)
 */
bool ai_init_from_env_file(const char *env_file_path) {
    FILE *env_file = NULL;
    char line[1024];
    char *key_value = NULL;
    bool found = false;
    
    if (env_file_path == NULL) {
        // Try multiple common locations for .env file
        const char *common_paths[] = {
            ".env",
            "data/.env",
            "../data/.env",
            "../../data/.env",
            "../.env"
        };
        
        for (size_t i = 0; i < sizeof(common_paths) / sizeof(common_paths[0]); i++) {
            fprintf(stderr, "Trying to load API key from: %s\n", common_paths[i]);
            env_file = fopen(common_paths[i], "r");
            if (env_file != NULL) {
                fprintf(stderr, "Found .env file at: %s\n", common_paths[i]);
                break;
            }
        }
        
        if (env_file == NULL) {
            fprintf(stderr, "Could not find a .env file in common locations\n");
            return false;
        }
    } else {
        env_file = fopen(env_file_path, "r");
        if (env_file == NULL) {
            fprintf(stderr, "Failed to open environment file: %s\n", env_file_path);
            return false;
        }
    }
    
    // Look for API key line in the file
    while (fgets(line, sizeof(line), env_file)) {
        // Remove newline if present
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
            len--;
        }
        
        // Skip empty lines
        if (len == 0) {
            continue;
        }
        
        // Skip comments
        if (line[0] == '#') {
            continue;
        }
        
        // Check if this line contains the API key
        if (strncmp(line, API_KEY_ENV_VAR, strlen(API_KEY_ENV_VAR)) == 0) {
            key_value = line + strlen(API_KEY_ENV_VAR);
            found = true;
            break;
        }
    }
    
    fclose(env_file);
    
    // If API key found, initialize with it
    if (found && key_value != NULL) {
        // Trim whitespace
        while (*key_value == ' ') {
            key_value++;
        }
        
        // Remove quotes if present
        if (*key_value == '"' || *key_value == '\'') {
            char quote = *key_value;
            key_value++;
            
            // Find matching end quote
            char *end_quote = strchr(key_value, quote);
            if (end_quote != NULL) {
                *end_quote = '\0';
            }
        }
        
        fprintf(stderr, "Found API key in .env file, initializing DeepSeek...\n");
        return ai_init(key_value);
    } else {
        fprintf(stderr, "API key not found in environment file\n");
        return false;
    }
}

bool ai_init(const char *key) {
    if (key == NULL || strlen(key) == 0) {
        fprintf(stderr, "Invalid API key provided\n");
        return false;
    }

    // Free previous key if exists
    if (api_key != NULL) {
        free(api_key);
    }

    // Store the API key
    api_key = strdup(key);
    if (api_key == NULL) {
        fprintf(stderr, "Failed to allocate memory for API key\n");
        return false;
    }

    // Initialize libcurl globally
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_global_init() failed: %s\n", curl_easy_strerror(res));
        free(api_key);
        api_key = NULL;
        return false;
    }

    return true;
}

void ai_cleanup(void) {
    if (api_key != NULL) {
        free(api_key);
        api_key = NULL;
    }
    
    // Cleanup curl
    curl_global_cleanup();
}

void ai_set_temperature(float temp) {
    if (temp >= 0.0f && temp <= 1.0f) {
        temperature = temp;
    } else {
        fprintf(stderr, "Temperature must be between 0.0 and 1.0\n");
    }
}

char *ai_generate_text(const char *prompt, const char *model_name) {
    if (api_key == NULL) {
        fprintf(stderr, "AI not initialized. Call ai_init first.\n");
        return NULL;
    }

    if (prompt == NULL || strlen(prompt) == 0) {
        fprintf(stderr, "Prompt cannot be empty\n");
        return NULL;
    }

    const char *model = model_name ? model_name : DEFAULT_MODEL;
    char *result = NULL;
    CURL *curl;
    CURLcode res;
    struct json_object *json_request, *messages, *message;
    struct json_object *json_response, *choices, *choice, *message_obj, *content_obj;
    const char *content_str;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  // will be grown as needed
    chunk.size = 0;            // no data at this point
    
    if (chunk.memory == NULL) {
        fprintf(stderr, "Failed to allocate memory for response\n");
        return NULL;
    }

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        
        // Create JSON request
        json_request = json_object_new_object();
        json_object_object_add(json_request, "model", json_object_new_string(model));
        json_object_object_add(json_request, "temperature", json_object_new_double(temperature));
        
        // Create messages array with user's message
        messages = json_object_new_array();
        message = json_object_new_object();
        json_object_object_add(message, "role", json_object_new_string("user"));
        json_object_object_add(message, "content", json_object_new_string(prompt));
        json_object_array_add(messages, message);
        
        // Add messages to request
        json_object_object_add(json_request, "messages", messages);
        
        // Convert JSON object to string
        const char *json_str = json_object_to_json_string(json_request);
        
        // Set HTTP headers
        headers = curl_slist_append(headers, "Content-Type: application/json");
        char auth_header[256];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
        headers = curl_slist_append(headers, auth_header);
        
        // Set curl options
        curl_easy_setopt(curl, CURLOPT_URL, DEEPSEEK_API_URL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        
        // Perform the request
        printf("Sending request to DeepSeek API...\n");
        res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Parse JSON response
            json_response = json_tokener_parse(chunk.memory);
            if (json_response != NULL) {
                // Extract the generated text - Use non-deprecated methods
                struct json_object_iterator it = json_object_iter_begin(json_response);
                struct json_object_iterator itEnd = json_object_iter_end(json_response);
                
                while (!json_object_iter_equal(&it, &itEnd)) {
                    const char *name = json_object_iter_peek_name(&it);
                    if (strcmp(name, "choices") == 0) {
                        choices = json_object_iter_peek_value(&it);
                        if (choices != NULL && json_object_is_type(choices, json_type_array)) {
                            choice = json_object_array_get_idx(choices, 0);
                            if (choice != NULL) {
                                struct json_object_iterator choice_it = json_object_iter_begin(choice);
                                struct json_object_iterator choice_itEnd = json_object_iter_end(choice);
                                
                                while (!json_object_iter_equal(&choice_it, &choice_itEnd)) {
                                    const char *choice_name = json_object_iter_peek_name(&choice_it);
                                    if (strcmp(choice_name, "message") == 0) {
                                        message_obj = json_object_iter_peek_value(&choice_it);
                                        
                                        if (message_obj != NULL) {
                                            struct json_object_iterator msg_it = json_object_iter_begin(message_obj);
                                            struct json_object_iterator msg_itEnd = json_object_iter_end(message_obj);
                                            
                                            while (!json_object_iter_equal(&msg_it, &msg_itEnd)) {
                                                const char *msg_name = json_object_iter_peek_name(&msg_it);
                                                if (strcmp(msg_name, "content") == 0) {
                                                    content_obj = json_object_iter_peek_value(&msg_it);
                                                    if (content_obj != NULL) {
                                                        content_str = json_object_get_string(content_obj);
                                                        if (content_str != NULL) {
                                                            result = strdup(content_str);
                                                        }
                                                    }
                                                }
                                                json_object_iter_next(&msg_it);
                                            }
                                        }
                                    }
                                    json_object_iter_next(&choice_it);
                                }
                            }
                        }
                        break;
                    }
                    json_object_iter_next(&it);
                }
                
                json_object_put(json_response); // Free JSON object
            } else {
                fprintf(stderr, "Failed to parse JSON response\n");
            }
        }
        
        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        json_object_put(json_request); // Free JSON request
    }
    
    // Free the response memory
    free(chunk.memory);
    
    return result;
}

void demo_ai_operations(void) {
    char api_key_input[256] = {0};
    char prompt[1024] = {0};
    char *ai_response = NULL;
    bool api_key_loaded = false;
    
    print_message("\n--- DeepSeek AI Integration Demo ---\n");
    
    // Try to load API key from .env file first
    const char *env_paths[] = {
        ".env",
        "data/.env",
        "../data/.env",
        "../../data/.env"
    };
    
    for (size_t i = 0; i < sizeof(env_paths) / sizeof(env_paths[0]); i++) {
        print_message("Trying to load API key from: ");
        print_message(env_paths[i]);
        print_message("\n");
        
        if (ai_init_from_env_file(env_paths[i])) {
            print_message("API key loaded successfully from file!\n");
            api_key_loaded = true;
            break;
        }
    }
    
    // Ask for API key if not already set
    if (!api_key_loaded && api_key == NULL) {
        print_message("Enter your DeepSeek API Key (or path to .env file): ");
        
        // Read API key
        if (fgets(api_key_input, sizeof(api_key_input), stdin) == NULL) {
            print_message("Failed to read API key\n");
            return;
        }
        
        // Remove newline character if present
        size_t len = strlen(api_key_input);
        if (len > 0 && api_key_input[len-1] == '\n') {
            api_key_input[len-1] = '\0';
        }
        
        // Check if user entered a file path instead
        if (strstr(api_key_input, ".env") != NULL) {
            // Try to load from the specified .env file
            if (ai_init_from_env_file(api_key_input)) {
                print_message("API key loaded successfully from file!\n");
            } else {
                print_message("Failed to load API key from file. Please enter the API key directly: ");
                
                // Read API key directly
                if (fgets(api_key_input, sizeof(api_key_input), stdin) == NULL) {
                    print_message("Failed to read API key\n");
                    return;
                }
                
                // Remove newline character if present
                len = strlen(api_key_input);
                if (len > 0 && api_key_input[len-1] == '\n') {
                    api_key_input[len-1] = '\0';
                }
                
                // Initialize AI with provided API key
                if (!ai_init(api_key_input)) {
                    print_message("Failed to initialize AI subsystem\n");
                    return;
                }
            }
        } else {
            // Initialize AI with provided API key
            if (!ai_init(api_key_input)) {
                print_message("Failed to initialize AI subsystem\n");
                return;
            }
        }
        
        print_message("AI initialized successfully\n");
    }
    
    // Set temperature
    float temp_value = 0.7f;
    print_message("Set temperature (0.0-1.0), default is 0.7: ");
    char temp_input[32] = {0};
    
    if (fgets(temp_input, sizeof(temp_input), stdin) != NULL) {
        // Remove newline character if present
        size_t temp_len = strlen(temp_input);
        if (temp_len > 0 && temp_input[temp_len-1] == '\n') {
            temp_input[temp_len-1] = '\0';
        }
        
        if (strlen(temp_input) > 0) {
            temp_value = (float)atof(temp_input);
            ai_set_temperature(temp_value);
            printf("Temperature set to: %.1f\n", temp_value);
        }
    }
    
    // Get user prompt
    print_message("\nEnter your prompt for DeepSeek AI: \n");
    if (fgets(prompt, sizeof(prompt), stdin) == NULL) {
        print_message("Failed to read prompt\n");
        return;
    }
    
    // Remove newline character if present
    size_t prompt_len = strlen(prompt);
    if (prompt_len > 0 && prompt[prompt_len-1] == '\n') {
        prompt[prompt_len-1] = '\0';
    }
    
    // Generate response from AI
    print_message("\nGenerating AI response...\n");
    ai_response = ai_generate_text(prompt, NULL);
    
    if (ai_response != NULL) {
        print_message("\n=== AI Response ===\n");
        print_message(ai_response);
        print_message("\n=== End of Response ===\n");
        
        // Free the response memory
        free(ai_response);
    } else {
        print_message("Failed to generate AI response\n");
    }
}