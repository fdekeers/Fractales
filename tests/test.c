#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int i = 1;
	char* line = argv[1];
	printf("Argument : %s\n",line);
	char* delim = " ";
	char* attr[5];
	attr[0] = strtok(line,delim);
	printf("Premier attribut : %s\n",attr[0]);
	while(i<5){
		attr[i] = strtok(NULL,delim);
		printf("Attribut %i : %s\n",i+1,attr[i]);
		i++;
	}
	return 0;
}