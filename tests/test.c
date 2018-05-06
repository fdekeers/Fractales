#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char *buffer[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
	for(int i=0;i<10;i++){
		if(buffer[i] != NULL){
			char* temp = buffer[i];
			buffer[i] = NULL;
			printf("Elément enlevé : %s\n",temp);
			printf("Elément à la position %i : %s\n",i+1,buffer[i]);
			return 0;
		}
	}
	printf("Tableau vide\n");
	return 1;
}