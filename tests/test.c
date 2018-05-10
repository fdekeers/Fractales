#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fractal.h"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <SDL.h>

pthread_mutex_t mutex;
sem_t empty;
sem_t full;
int global = 0;
struct fractal **buffer;
int bufsize = 4;


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

int add_buffer(struct fractal* frac, struct fractal** buffer){
    
	for(int i = 0; i < bufsize; i++){
        
		if(*(buffer+i) == NULL){
			*(buffer+i) = frac; // On ajoute la fractale frac au buffer a l'emplacement i
			break;
		}
	}
	return 0;
}


struct fractal* remove_buffer(struct fractal** buffer){
    
	for(int i=0;i<bufsize;i++){
        
        // Si il y a un element a l'emplacement i du buffer, on retourne cet element qui est une fractale et on met l'emplacement du buffer a NULL
		if(*(buffer+i) != NULL){
			struct fractal* temp = (*buffer+i);
			*(buffer+i) = NULL;
			return temp;
            
		}
	}
	return NULL;
}


double calcul_moyenne(struct fractal *frac){
    
	double r;
	unsigned int width = fractal_get_width(frac); // Variable qui contient la largeur de la fractale
	unsigned int height = fractal_get_height(frac); // Variable qui contient la hauteur de la fractale
    
    // On parcourt tous les pixels
	for(unsigned int i = 1; i <= width; i++){
        
		for(unsigned int j = 1; j <= height; j++){
            
			r = r + fractal_compute_value(frac,i,j);
		}
	}
	return r/(width*height);
}
	

void* func(struct arg_struct* args){
	struct fractal* frac = create_fractal(args->line);
	pthread_mutex_lock(&mutex);
	int err = add_buffer(frac,args->buffer);
	pthread_mutex_unlock(&mutex);
}


int main(int argc, char *argv[]) {
	
	struct fractal** buffer = (struct fractal**)malloc(sizeof(struct fractal*)*4);
	for(int i=0;i<4;i++){
		*(buffer+i) = NULL;
	}
	
	int err = pthread_mutex_init(&(mutex),NULL);
	err = sem_init(&empty,0,10);
	err = sem_init(&full,0,0);
	
	// Affichage des valeurs du buffer
	for(int i=0;i<4;i++){
		if(*(buffer+i) == NULL){
			printf("Fractale à la position %i : NULL\n",i+1);
		}
		else{
			printf("Fractale à la position %i : %s\n",i+1,fractal_get_name(*(buffer+i)));
		}
	}
	
	pthread_t threads[4];
	for(int i=0;i<4;i++){
		struct arg_struct* args = (struct arg_struct*)malloc(sizeof(struct arg_struct));
		args->line = argv[i+1];
		args->buffer = buffer;
		err = pthread_create(&(threads[i]),NULL,&func,args);
		if(err != 0){
			printf("Erreur lors de la création des threads\n");
			return 1;
		}
	}
	printf("Threds créés\n");
	
	for(int i=0;i<4;i++){
		err = pthread_join(threads[i],NULL);
		if(err != 0){
			printf("Erreur dans la fonction pthread_join\n");
			return 1;
		}
	}
	printf("Threads join\n");
	
	// Affichage des valeurs du buffer
	for(int i=0;i<4;i++){
		if(*(buffer+i) == NULL){
			printf("Fractale à la position %i : NULL\n",i+1);
		}
		else{
			printf("Fractale à la position %i : %s\n",i+1,fractal_get_name(*(buffer+i)));
		}
	}
	return 0;
}

