#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fractal.h"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#define ERROR = 1

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
	if(buf == NULL){
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
        if (buffer[0]=='#' || buffer==NULL){
            break;
        }
        readline();
        rd = read(fd, buffer, sizeof(char));
    }
}


// Lecture d'une ligne sur un fichier
// Retourne 0 si le fichier n'est pas fini, et 1 si le fichier est fini
// OK !
int readline(FILE* stream, char* buf){
	int i = 0;
	char temp = fgetc(stream);
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
	attr[0] = strtok(line,delim);
	while(i<5){
		attr[i] = strtok(NULL,delim);
		i++;
	}
	return fractal_new(attr[0],atoi(attr[1]),atoi(attr[2]),atof(attr[3]),atof(attr[4]));
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

// On compare 2 fractales pour savoir laquelle a la plus grande valeur, si on applique cette fonction sur l'entierete d'un tableau, on peut trouver le max de toutes les fractales.
void max_fractale(struct fractal *frac){
	moy = calcul_moyenne(frac)
	if(moy>max){
		max = moy;
		frac_max = frac;
	}
}

// Calcule la moyenne des valeurs de toutes les fractales
// Quels arguments ? Comment trouver le nombre de fractales ?
/*
void moyenne(){
    struct fractal *f = (struct fractal)malloc(sizeof(struct fractal)); // Un tableau de fractales, on a egalement acces a int **values qui stocke toutes les valeurs qui concerne la fractale
    double somme_iter;
    double somme_values;
    double moyenne;
    for (int i=0; ;i++){
        int nbr_iter = fractal_compute_value(f[i],f[i]->a,f[i]->b);
        somme_iter += nbr_iter;
        somme_values += fractal_get_value(f[i],f[i]->a,f[i]->b);
    }
    moyenne = somme_values/somme_iter;
}
*/

int calcul_moyenne(const struct fractale *frac){
	unsigned long sum;
	unsigned int width = fractal_get_width(frac);
	unsigned int height = fractal_get_height(frac);
	for(int i = 0;i<width;i++){
		for(int j = 0;j<height;j++){
			sum = sum + fractal_compute_value(frac,i,j);
		}
	}
	return sum/(width*height);
}