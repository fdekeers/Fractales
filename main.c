#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fractal.h"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#define ERROR = 1

#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUCurses.h>

// Initialisations
double max;
struct fractal *frac_max = NULL;
struct fractal *buffer[10];
pthread_mutex_t mutex;
pthread_mutex_init(&mutex,NULL);
sem_t empty;
sem_init(&empty,0,10);
sem_t full;
sem_init(&full,0,0);

int main(int argc, char *argv[])
{
	char* destination = argv[argc-1];
	int d = 0;
	int maxthreads;
	for(int i = 1;i<argc;i++){
		if(argv[i] == "-d"){
			d = 1;
		}
		if(argv[i] == "--maxthreads"){
			maxthreads = atoi(argv[i+1]);
		}
	}
	pthread_t thread;
    int retour_thread = pthread_create(&thread,NULL,void *(*start_routine) (void *), void *arg);
    if (retour_thread != 0){
        printf("Erreur lors de la création du thread\n");
    }
	return 0;
}

// Threads de lecture
int producteur(char* filename){
    FILE *stream = fopen(filename,"r");
	if(stream == NULL){
		return 1;
	}
	char* line = (char*)malloc(sizeof(char)*100);
	if(line == NULL){
		return 1;
	}
	printf("Succès de la fonction fopen\n");
	int done = 0;
	while(done == 0){
		done = readline(stream,line);
		struct fractal* fra = create_fractal(line);
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		add_buffer(fra);
		pthread_mutex_unlock(&mutex);
        sem_post(&full);
	}
}

// Threads de calcul
// Le consommateur va prendre les données du producteur (le buffer)
int consommateur (){
	int err;
	struct fractal* frac = (struct fractal*)malloc(sizeof(struct fractal));
    if (frac == NULL){
        return 1;
    }
    while (true){
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
		frac = remove_buffer();
        max_fractale(frac);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
		max_fractale(frac);
    }
}


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

// Creation d'une fractale sur base d'une ligne
// OK !
struct fractal* create_fractal(*char line){
	int i = 1;
	char* delim = " ";
	char* attr[5];
	attr[0] = strtok(line,delim); // On split la ligne à chaque espace qu'on rencontre
	while(i<5){
		attr[i] = strtok(NULL,delim);
		i++;
	}
	return fractal_new(attr[0],atoi(attr[1]),atoi(attr[2]),atof(attr[3]),atof(attr[4])); // Creation d'une nouvelle fractale
}

// Ajouter un élément au buffer
// OK !
int add_buffer(struct fractal *frac){
	for(int i=0;i<10;i++){
		if(buffer[i] == NULL){
			buffer[i] = frac;
			return 0;
		}
	}
	return 1;
}

// Retirer un élément du buffer
// OK !
struct fractal* remove_buffer(){
	for(int i=0;i<10;i++){
		if(buffer[i] != NULL){
			struct fractal* temp = buffer[i];
			buffer[i] = NULL;
			return temp;
		}
	}
	return NULL;
}

// On trouve la fractale qui a la valeur moyenne la plus grande
void max_fractale(struct fractal *frac){
	moy = calcul_moyenne(frac)
	if(moy>=max){
        max = moy;
		frac_max = frac;
        fract_push(*frac_max, max);
	}
}

// On calcule la moyenne des valeurs de la fractale en chaque pixel
int calcul_moyenne(struct fractale *frac){
	unsigned long sum;
	unsigned int width = fractal_get_width(frac);
	unsigned int height = fractal_get_height(frac);
	for(int i = 0;i<width;i++){
		for(int j = 0;j<height;j++){
			sum += fractal_compute_value(frac,i,j);
		}
	}
	return sum/(width*height);
}

// Fonction qui va afficher toutes les fractales qui sont présentes dans la pile
void frac_affiche(struct noeud * head){
    if (head == NULL){
        return NULL;
    }
    int i=0;
    char *fname;
    struct noeud * runner = head;
    while (runner != NULL){
        fname = "BMP_out_i";
        write_bitmap_sdl(runner->fract, fname);
        i++;
        runner = runner->next;
    }
}
