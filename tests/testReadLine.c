#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	char* filename = argv[1];
	int i = 0;
	printf("L'argument est : %s\n",filename);
	printf("Succès de la fonction malloc\n");
	FILE* stream = fopen(filename,"r");
	if(stream == NULL){
		printf("Erreur dans la fonction fopen\n");
		return 1;
	}
	printf("Succès de la fonction fopen\n");
	char* buf = (char*)malloc(sizeof(char)*84);
	if(buf == NULL){
		printf("Erreur readline/malloc\n");
		return 1;
	}
	char temp = fgetc(stream);
	while(temp != '\n'){
		*(buf+i) = temp;
		temp = fgetc(stream);
		i++;
	}
	printf("Ligne lue : %s\n",buf);
	int err = fclose(stream);
	if(err != 0){
		printf("Erreur dans la fonction fclose\n");
		free(buf);
		return 1;
	}
	printf("Succès de la fonction fclose\n");
	free(buf);
	printf("Succès de la fonction free\n");
	return 0;
}