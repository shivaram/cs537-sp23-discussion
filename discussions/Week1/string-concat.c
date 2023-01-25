#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
	char str[30];
	strcpy(str, "Hello");
	strcat(str, " World!");
	printf("str = %s\n", str);
	return 0;
}
