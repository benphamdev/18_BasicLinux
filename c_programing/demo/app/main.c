#include "../lib/syscalls.h"
#include "../src/demos.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Forward declaration for signal handler in main app
void signal_handler(int signum);

// Static variable for signal handling
static volatile sig_atomic_t signal_received = 0;

// Signal handler function implementation
void signal_handler(int signum) {
    signal_received = 1;
    printf("\nReceived signal %d (SIGINT)\n", signum);
}

void diplay_menu() {
    print_message("\n------ DEMO MENU ------\n");
    print_message("1. File Operations Demo\n");
    print_message("2. Process Operations Demo\n");
    print_message("3. Directory Operations Demo\n");
    print_message("4. Pipe Operations Demo\n");
    print_message("5. Time Operations Demo\n");
    print_message("6. Signal Handling Demo\n");
    print_message("7. Memory Management Demo\n");
    print_message("8. File Permission Demo\n");
    print_message("9. Basic I/O Demo\n");
    print_message("10. System Information Demo\n");
    print_message("11. User and Group Management Demo\n");
    print_message("12. Filesystem Operations Demo\n");
    print_message("13. Terminal I/O Demo\n");
    print_message("14. Message Queue Demo\n");
    print_message("15. Event Monitoring Demo\n");
    print_message("16. File Monitoring Demo\n");
    print_message("17. Process Scheduling Demo\n");
    print_message("18. Advanced File Operations Demo\n");
    print_message("19. Extended Attributes Demo\n");
    print_message("20. Advanced Memory Management Demo\n");
    print_message("21. Advanced Networking Demo\n");
    print_message("22. System Timers Demo\n");
    print_message("23. Process Priority and Resource Usage Demo\n");
    print_message("24. User and Group IDs Demo\n");
    print_message("25. File Locking Demo\n");
    print_message("0. Exit Program\n");
    print_message("----------------------\n");
    print_message("Enter your choice: ");
}

int main() {
    int choice = 0;
    char input[10];
    
    // Display program header
    print_message("\n===== System Call Library Demo =====\n");
    print_message("This program demonstrates various system call wrappers\n\n");
    
    while (1) {
        // Display menu
        diplay_menu();
        
        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        // Convert input to integer
        choice = atoi(input);
        
        // Process user choice
        switch (choice) {
            case 1:
                demo_file_operations();
                break;
            case 2:
                demo_process_operations();
                break;
            case 3:
                demo_directory_operations();
                break;
            case 4:
                demo_pipe_operations();
                break;
            case 5:
                demo_time_operations();
                break;
            case 6:
                demo_signal_operations();
                break;
            case 7:
                demo_memory_operations();
                break;
            case 8:
                demo_file_permission_operations();
                break;
            case 9:
                print_message("\n--- Basic I/O Demo ---\n");
                print_message("Enter a message: ");
                char *message = read_line();
                print_message("You entered: ");
                print_message(message);
                free(message); // Free allocated memory
                break;
            case 10:
                demo_system_info_operations();
                break;
            case 11:
                demo_user_group_operations();
                break;
            case 12:
                demo_filesystem_operations();
                break;
            case 13:
                demo_terminal_operations();
                break;
            case 14:
                demo_message_queue_operations();
                break;
            case 15:
                demo_event_monitoring();
                break;
            case 16:
                demo_file_monitoring();
                break;
            case 17:
                demo_process_scheduling();
                break;
            case 18:
                demo_advanced_file_operations();
                break;
            case 19:
                demo_extended_attributes();
                break;
            case 20:
                demo_advanced_memory_management();
                break;
            case 21:
                demo_advanced_networking();
                break;
            case 22:
                demo_system_timers();
                break;
            case 23:
                demo_process_priority();
                break;
            case 24:
                demo_user_group_ids();
                break;
            case 25:
                demo_file_locking();
                break;
            case 0:
                print_message("\nExiting System Call Library Demo. Goodbye!\n");
                return 0;
            default:
                print_message("\nInvalid choice. Please try again.\n");
                break;
        }
        
        // Pause after each demo (except exit)
        if (choice != 0) {
            print_message("\nPress Enter to return to menu...");
            getchar(); // Consume any remaining newline from previous input
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }
        }
    }
    
    return 0;
}