
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
    //strdup save a copy of a string
    char *str = strdup("This is a line");
   
    //strsep doest not wotk on constant string
    char *tok = strsep(&str, " ");

    while(tok != NULL){
        printf("%s\n", tok);
        tok = strsep(&str, " ");
    }
    free(str);
    return 0;
}