#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char *reverse(char *str){
   char *tmp;
   int i, j;
   j = 0;
   tmp = (char *) malloc(strlen(str) + 1);
   for(i = strlen(str) - 1; i >= 0; i--){
      tmp[j++] = str[i];
   }
   
   return tmp;
}

int main(int argc, char *argv[]){

    char str[100] = "Hello World";
    printf("str: %s\n", str);

    // Reverse the string
    char *res = reverse(str);

    // Print the result
    printf("Reverse of str: %s\n", res);

    return 0;
}
