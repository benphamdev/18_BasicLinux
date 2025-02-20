#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: %s <num1> <num2>\n", argv[0]);
        return 1;
    }

    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
    int c = num1 + num2;

    printf("Addition: %d\n", c);
    return 0;
}