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
struct noeud **head;


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
	struct noeud* noeud = createNoeud(frac);
	return push(head,noeud);
}


struct fractal* remove_buffer(){
    return pop(head);
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
	

void* producteur(struct arg_struct* args){
	int* error = (int*)malloc(sizeof(int));
	int* success = (int*)malloc(sizeof(int));
	*error = 1;
	*success = 0;
    FILE *stream = fopen(args->line,"r");
    
    // On teste si la fonction fopen a echoue
	if(stream == NULL){
		return error;
	}
    
    
	int done = 0;
	int i = 0;
	int err;
	
	while(done == 0){
		char* line = (char*)malloc(sizeof(char)*100);
		// On teste si le malloc renvoie une erreur ou ne s'est pas execute correctement
		if(line == NULL){
			return error;
		}
		done = readline(stream,line);
		printf("Done = %i\n",done);
		if(done != 2){
		printf("Ligne lue : %s\n",line);
			struct fractal* fra = create_fractal(line);
			sem_wait(&empty); // On fait attendre la premiere semaphore
			pthread_mutex_lock(&mutex); // On protege la section critique avec un mutex qu'on bloque
        	// Section critique
		
			err = add_buffer(fra);
			printf("Fractale ajoutée au buffer\n");
        	// Fin de la section critique
			pthread_mutex_unlock(&mutex); // On debloque le mutex
        	sem_post(&full);
			i++;
		}
	}
	err = fclose(stream);
    return success;
}

int main(int argc, char *argv[]) {
	
	
    head = (struct noeud**)malloc(sizeof(struct noeud*));
	*head = (struct noeud*)malloc(sizeof(struct noeud));
	
	
	int nfichiers = argc-2;
	char* destination = argv[argc-1];
	int d = 0;
	int maxthreads = 5;
	
	// Prise en compte des arguments de la main
	for(int i = 1;i<argc;i++){
        
		if(strcmp(argv[i],"-d") == 0){
			d = 1;
			nfichiers = nfichiers-1;
		}
        
		if(strcmp(argv[i],"--maxthreads") == 0){
			maxthreads = atoi(argv[i+1]);
			nfichiers = nfichiers-2;
		}
	}
	
	printf("Nombre de fichiers à lire : %i\n",nfichiers);
	
	// Création du tableau de fichiers d'entrée
	char* fichiers[nfichiers];
	int i = 1;
	int j = 0;
    
	while(i<argc-1 && j<nfichiers){
        
		if(strcmp(argv[i],"-d") == 0){
			i++;
		}
        
		else if(strcmp(argv[i],"--maxthreads") == 0){
			i = i+2;
		}
        
		else{
			fichiers[j] = argv[i];
			i++;
			j++;
		}
	}
	
	for(int i = 0;i<nfichiers;i++){
		printf("Fichier n.%i : %s\n",i+1,fichiers[i]);
	}
	
	int err = pthread_mutex_init(&(mutex),NULL);
	err = sem_init(&empty,0,bufsize);
	err = sem_init(&full,0,0);
	
	struct arg_struct **args = (struct arg_struct**)malloc(sizeof(struct arg_struct*)*nfichiers);
	for(int i=0;i<nfichiers;i++){
		*(args+i) = (struct arg_struct*)malloc(sizeof(struct arg_struct));
	}
	printf("coucou\n");
	pthread_t threads[nfichiers];
	printf("coucou\n");
	for(int i = 0;i<nfichiers;i++){
		(*(args+i))->line = fichiers[i];
		(*(args+i))->head = head;
		err = pthread_create(&(threads[i]),NULL,&producteur,*(args+i));
		if(err != 0){
			printf("Erreur lors de la création des threads de lecture\n");
			return 1;
		}
	}

	for(int i=0;i<nfichiers;i++){
		err = pthread_join(threads[i],NULL);
		if(err != 0){
			printf("Erreur dans la fonction pthread_join\n");
			return 1;
		}
	}
	
	printf("Head : %s\n",fractal_get_name((*head)->fract));
	
	sem_destroy(&full);
	sem_destroy(&empty);
	
	
	return 0;
		
}