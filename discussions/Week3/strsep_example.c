
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
    //strdup save a copy of a string
    char *str = strdup("This is a line");
   
    //strsep doest not work on constant string

    char *tok = str, *tmp = str; 

    while((tok = strsep(&tmp, " ")) != NULL){
	printf("%s\n", tok);
    }

    free(str);
    return 0;
}
