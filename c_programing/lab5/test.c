//This Proram give Segmentation fault
#include<stdio.h>

#include<stdlib.h>

int main() {
  int * ptr = NULL;
  * ptr = 1;
  printf("value =%d", * ptr);
  return 0;
}
