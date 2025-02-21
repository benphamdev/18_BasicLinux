#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int r, i;
    char p[256]; // Increased buffer size to handle longer paths

    if (argc < 3) {
        printf("Improper arguments\nFile names required\n");
        return 1;
    } else if (argc == 3) {
        printf("\nSource: %s\nDestination: %s\n", argv[1], argv[2]);
        r = link(argv[1], argv[2]);
        if (r == 0) {
            unlink(argv[1]);
        } else {
            perror("Error linking files");
            return 1;
        }
    } else {
        for (i = 1; i < argc - 1; i++) {
            snprintf(p, sizeof(p), "%s/%s", argv[argc - 1], argv[i]);
            printf("Moving %s to %s\n", argv[i], p);
            r = link(argv[i], p);
            if (r == 0) {
                unlink(argv[i]);
            } else {
                perror("Error linking files");
                return 1;
            }
        }
    }
    return 0;
}