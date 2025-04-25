#ifndef AI_INTEGRATION_H
#define AI_INTEGRATION_H

/**
 * @file ai_integration.h
 * @brief DeepSeek AI integration functions
 */

#include <stdbool.h>

/**
 * @brief Initialize the AI subsystem with the provided API key
 * @param api_key The DeepSeek API key to use for authentication
 * @return True if initialization was successful, false otherwise
 */
bool ai_init(const char *api_key);

/**
 * @brief Read API key from environment file and initialize AI
 * @param env_file_path Path to the environment file containing the API key
 * @return True if initialization was successful, false otherwise
 */
bool ai_init_from_env_file(const char *env_file_path);

/**
 * @brief Cleanup and free resources used by the AI subsystem
 */
void ai_cleanup(void);

/**
 * @brief Send a prompt to DeepSeek AI and get a response
 * @param prompt The user's prompt to send to the AI
 * @param model_name Optional model name (NULL for default)
 * @return AI-generated response (caller must free this memory)
 */
char *ai_generate_text(const char *prompt, const char *model_name);

/**
 * @brief Set temperature for AI generation (controls randomness)
 * @param temp Temperature value between 0.0 and 1.0
 */
void ai_set_temperature(float temp);

/**
 * @brief Run an interactive AI demo using DeepSeek
 */
void demo_ai_operations(void);

#endif /* AI_INTEGRATION_H */