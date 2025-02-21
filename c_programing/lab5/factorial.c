#include<stdio.h>

int main() {
  int i, num, result;
  printf("enter the number: ");
  scanf("%d", & num);
  for (i = 1; i <= num; i++) {
    result = i * result;
  }
  printf("factorial of the number %d =%d\n", num, result);
  return 0;
}
