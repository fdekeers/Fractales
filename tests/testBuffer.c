#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fractal.h"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <SDL.h>


struct fractal *buffer[10] = {NULL};

int readline(FILE* stream, char* buf){
    
	int i = 0;
	char temp = fgetc(stream);
	if(temp == EOF){
		return 2;
	}
	
	// Lignes qui doivent être passées 
	while(temp == '\n' || temp == '#'){
		
		// Ligne vide
		while(temp == '\n'){
        	temp = fgetc(stream);
		}
		if(temp == EOF){
			return 2;
		}
		
		// Ligne commençant par '#'
		while(temp == '#'){
        
			while(temp != '\n' && temp != EOF){
				temp = fgetc(stream);
			}
			if(temp == EOF){
				return 2;
			}
			temp = fgetc(stream);
        
		}
	}
    
	while(temp != '\n' && temp != EOF){
		*(buf+i) = temp;
		temp = fgetc(stream);
		i++;
	}
    
	if(temp == EOF){
		return 1; // Fichier termine
	}
    
	return 0; // Fichier non termine
}



struct fractal* create_fractal(char* line){
	int i = 1;
	char* delim = " "; // On definit le caractere duquel on va se servir pour split notre ligne
	char* attr[5];
	attr[0] = strtok(line,delim); // On split la ligne à chaque espace qu'on rencontre
    
	while(i<5){
		attr[i] = strtok(NULL,delim); //  On cree un tableau avec, dans chaque emplacement, un element qui va nous permettre de creer une fractale
		i++;
	}
    
	return fractal_new(attr[0],atoi(attr[1]),atoi(attr[2]),atof(attr[3]),atof(attr[4])); // Creation d'une nouvelle fractale
}


int add_buffer(struct fractal* frac){
    
	for(int i = 0; i < 10; i++){
        
		if(buffer[i] == NULL){
			buffer[i] = frac; // On ajoute la fractale frac au buffer a l'emplacement i
			return 0;
		}
	}
	return 1;
}


struct fractal* remove_buffer(){
    
	for(int i=0;i<10;i++){
        
        // Si il y a un element a l'emplacement i du buffer, on retourne cet element qui est une fractale et on met l'emplacement du buffer a NULL
		if(buffer[i] != NULL){
			struct fractal* temp = buffer[i];
			buffer[i] = NULL;
			return temp;
            
		}
	}
	return NULL;
}



int main(int argc, char *argv[]) {
	for(int i = 0;i<10;i++){
		printf("Elément %i du buffer : %s\n",i+1,fractal_get_name(buffer[i]));
	}
	char* fname = argv[1];
	FILE* stream = fopen(fname,"r");
	char* line = (char*)malloc(sizeof(char)*100);
	int done = readline(stream,line);
	struct fractal* frac = create_fractal(line);
	int err = add_buffer(frac);
	for(int i = 0;i<10;i++){
		printf("Elément %i du buffer : %s\n",i+1,fractal_get_name(buffer[i]));
	}
	done = readline(stream,line);
	for(int i = 0;i<10;i++){
		printf("Elément %i du buffer : %s\n",i+1,fractal_get_name(buffer[i]));
	}
	frac = create_fractal(line);
	err = add_buffer(frac);
	for(int i = 0;i<10;i++){
		printf("Elément %i du buffer : %s\n",i+1,fractal_get_name(buffer[i]));
	}
	return 0;
}
