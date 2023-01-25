#include <stdio.h>

int main(){
    char name[30];
    printf("Enter name: ");
    fgets(name, 30, stdin);  
    printf("Name: ");
    puts(name); 
    return 0;
}
