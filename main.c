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
        readline();
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

// Threads de calcul
// Pour calculer la moyenne de tous les threads pour calculer la valeur de la fractale max
void consommateur (){
    long calcul;
    while (true){
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        // Section critique
        max_thread(empty,full);
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
        if (buffer[0]=='#' || buffer==NULL){
            break;
        }
        readline();
        rd = read(fd, buffer, sizeof(char));
    }
}

// Lecture d'une ligne sur un fichier
// Retourne le buffer qui contient une ligne du fichier
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
// Retourne la nouvelle fractale
// OK !
struct fractal* create_fractal(*char line){
	int i = 1;
	char* delim = " ";
	char* attr[5];
	attr[0] = strtok(line,delim); // On split la ligne a chaque espace
	while(i<5){
		attr[i] = strtok(NULL,delim);
		i++;
	}
	return fractal_new(attr[0],atoi(attr[1]),atoi(attr[2]),atof(attr[3]),atof(attr[4])); // Creation d'une nouvelle fractale
}


// On compare 2 fractales pour savoir laquelle a la plus grande valeur, si on applique cette fonction sur l'entierete d'un tableau, on peut trouver le max de toutes les fractales.
void max_fractale (struct fractal *f1, struct fractal *f2){
    int max; // Fractale max
    int nbr_iter1 = fractal_compute_value(f1,f1->a,f1->b); // Nombre d'iterations de la valeur de recurrence sur la fractale
    int nbr_iter2 = fractal_compute_value(f2,f2->a,f2->b);
    long valeur_f1 = f1->values/nbr_iter1;
    long valeur_f2 = f2->values/nbr_iter2;
    if (valeur_f1 > max){
        max = valeur_f1;
    }
    else if (valeur_f2 > f2){
        max = valeur_f2;
    }
    else
        max = max;
}

// Calcule la moyenne des valeurs de toutes les fractales
// Quels arguments ? Comment trouver le nombre de fractales ?
void moyenne (){
    struct fractal *f = (struct fractal)malloc(sizeof(struct fractal)); // Un tableau de fractales, on a egalement acces a int **values qui stocke toutes les valeurs qui concerne la fractale
    double somme_iter;
    double somme_values;
    double moyenne;
    for (int i=0; ;i++){
        int nbr_iter = fractal_compute_value(f[i],f[i]->a,f[i]->b);
        somme_iter += nbr_iter;
        somme_values += f[i]->values;
    }
    moyenne = somme_values/somme_iter;
}
