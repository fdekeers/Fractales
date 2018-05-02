#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fractal.h"
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#define ERROR = 1

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

// Lecture d'une ligne sur un fichier
// OK !
char* readline(FILE* stream){
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
	return buf;
}

// Creation d'une fractale sur base d'une ligne
// OK !
struct fractal* create_fractal(*char line){
	int i = 1;
	char* delim = " ";
	char* attr[5];
	attr[0] = strtok(line,delim);
	while(i<5){
		attr[i] = strtok(NULL,delim);
		i++;
	}
	return fractal_new(attr[0],atoi(attr[1]),atoi(attr[2]),atof(attr[3]),atof(attr[4]));
}


// Calcul
void calculer (){
    
}
