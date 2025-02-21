#include "../lib/syscalls.h"

int main() {
    print_message("Enter a message: ");
    char *message = read_line();
    print_message("You entered: ");
    print_message(message);

    free(message); // Giải phóng bộ nhớ đã cấp phát
    return 0;
}