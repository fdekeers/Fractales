#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fractal.h"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <SDL.h>

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

int main(int argc, char *argv[]) {
	char* fname = argv[1];
	FILE* stream = fopen(fname,"r");
	if(stream == NULL){
		return 1;
	}
	char* line = (char*)malloc(sizeof(char)*100);
	int done = readline(stream,line);
	while(done == 0){
		printf("Ligne lue : %s\n",line);
		struct fractal* frac = create_fractal(line);
		printf("Nom de la fractale : %s\n",fractal_get_name(frac));
		printf("Largeur : %i\n",fractal_get_width(frac));
		printf("Hauteur : %i\n",fractal_get_height(frac));
		printf("a = %f\n",fractal_get_a(frac));
		printf("b = %f\n",fractal_get_b(frac));
		done = readline(stream,line);
	}
	if(done == 1){
		printf("Ligne lue : %s\n",line);
		struct fractal* frac = create_fractal(line);
		printf("Nom de la fractale : %s\n",fractal_get_name(frac));
		printf("Largeur : %i\n",fractal_get_width(frac));
		printf("Hauteur : %i\n",fractal_get_height(frac));
		printf("a = %f\n",fractal_get_a(frac));
		printf("b = %f\n",fractal_get_b(frac));
	}
	printf("Fin du fichier\n");
	int err = fclose(stream);
	if(err != 0){
		return 1;
	}
	return 0;
}
