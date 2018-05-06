#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

// Lecture d'une ligne sur un fichier
// Retourne 0 si le fichier n'est pas fini, et 1 si le fichier est fini
// OK !
int readline(FILE* stream, char* buf){
	int i = 0;
	char temp = fgetc(stream);
	while(temp == '\n'){
		temp = fgetc(stream);
	}
	while(temp == '#'){
		while(temp != '\n' && temp != EOF){
			temp = fgetc(stream);
			i++;
		}
	}
	i = 0;
	while(temp != '\n' && temp != EOF){
		*(buf+i) = temp;
		temp = fgetc(stream);
		i++;
	}
	if(temp == EOF){
		return 1;
	}
	return 0;
}






int main(int argc, char *argv[]) {
	char* file = argv[1];
	char* buf = (char*)malloc(sizeof(char)*100);
	if(buf == NULL){
		return 1;
	}
	FILE* stream = fopen(file,"r");
	if(stream == NULL){
		return 1;
	}
	int done = readline(stream,buf);
	while(done == 0){
		printf("Ligne lue : %s\n",buf);
		done = readline(stream,buf);
	}
	printf("Ligne lue : %s\n",buf);
	return 0;
}