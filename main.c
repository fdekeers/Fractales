#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fractal.h"
#include <semaphore.h>
#include <fcnt1.h>

int main()
{
    /* TODO */
	printf("Coucou les zamis\n");
    return 0;
}

// Initialisations
struct fractal buffer = (struct fractal)malloc(sizeof(struct fractal) * maxthreads);
struct fractal fract_table [1000];
pthread_mutex_t mutex;
sem_t empty; // Sert a compter le nombre de slots qui sont vides dans le buffer partagé
sem_t full; // Sert a compter le nombre de slots qui sont remplis dans le buffer partagé
pthread_mutex_init(&mutex,NULL);
sem_init(&empty,maxthreads); // Initialisé à la longueur du buffer
sem_init(&full,0); // Initialisé à 0

// Threads de lecture
void producteur (){
    char lecture;
    while (true){
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        // Section critique
        reader();
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

// Threads de calcul
void consommateur (){
    long calcul;
    while (true){
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        // Section critique
        calculer();
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

// Lecture
void reader (){
    int fd = open(0,O_RDONLY);
    if (fd == -1){
        return -1;
    }
    char * buff;
    int rd = read(fd, buf, sizeof(char));
    for (int i = 0; rd != -1 && buffer[i] != '\n'; i++) {
        rd = read(fd, buffer, sizeof(char));
    }
    if (buffer[0]=='#' || buffer==NULL){
        break;
    }
    char * tab = strtok(buffer, ' '); // Split la ligne qui a été lue
    while (tab != NULL){
        tab = strtok(NULL, ' ');
    }
}

// Lecture d'une ligne
char* readline(int fd){
	char line;
	char buf;
	ssize_t size = read(fd, (void*)&buf, sizeof(char));
	if(*buf == '\n' || *buf == '#'){
		return NULL;
	}
	for(int i = 0; i<1000; i++){
		size = read(fd, (void*)((&buf)+i), sizeof(char);
		if(*(buf+i+1) == '\n'){
			return &buf;
		}
	}
	return NULL;
}

// Creation d'une fractale sur base d'une ligne
struct fractal* create_fractal(*char line){
	int i = 0;
	struct fractal *fractal = (struct fractal*)malloc(sizeof(struct fractal));
	char* delim = " ";
	char* token;
	char* attr[];
	token = strtok(line, delim);
	while(token != NULL) {
		attr[i] = token;
		token = strtok(NULL, delim);
	}
	fractal = fractal_new(attr[0],attr[1],attr[2],attr[3],attr[4]); //il faut caster
	return fractal;
}


// Calcul
void calculer (){
    
}
