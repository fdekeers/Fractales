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

/*
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
*/

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


void* func(){
	for(int i = 0;i<1000;i++){
		pthread_mutex_lock(&mutex);
		global++;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
	

int main(int argc, char *argv[]) {
	struct noeud** head = (struct noeud**)malloc(sizeof(struct noeud*));
	char* line = argv[1];
	struct fractal* frac = create_fractal(line);
	printf("Nom de la fractale : %s\n",fractal_get_name(frac));
	printf("Largeur : %i\n",fractal_get_width(frac));
	printf("Hauteur : %i\n",fractal_get_height(frac));
	printf("a = %f\n",fractal_get_a(frac));
	printf("b = %f\n",fractal_get_b(frac));
	struct noeud* noeud = createNoeud(frac);
	*head = noeud;
	printf("Fractale dans le noeud : %s\n",fractal_get_name(noeud->fract));
	printf("Fractale dans le noeud : %s\n",fractal_get_name((*head)->fract));
	line = argv[2];
	frac = create_fractal(line);
	int err = push(head,createNoeud(frac));
	printf("Fractale dans le noeud 2 : %s\n",fractal_get_name((*head)->fract));
	return 0;
}
