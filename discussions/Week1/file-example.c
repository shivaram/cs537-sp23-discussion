#include <stdio.h>

int main () {
   FILE *f;
   char str[60];
   char *res;
   /* opening file for reading */
   f = fopen("name.txt" , "r");
   if(f == NULL) {
      perror("Error opening file");
      return -1;
   }
   
   res = fgets(str, 60, f);
   
   if(res != NULL ) {
      /* writing content to stdout */
      printf("%s", str);
   }
   
   fclose(f);
   
   return(0);
}
