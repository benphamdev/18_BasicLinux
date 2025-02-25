#include <stdio.h>
int main(void) {
 int i, my_array[8];
 for(i=0;i<=20;i++) {
 my_array[i] = i+1;
 }
 for(i=0;i<8;i++) {
 printf("%d\n", my_array[i]);
 }
 return 0;
}
