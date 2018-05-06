// @Titre : Projet Fractales
// @Auteurs : Francois DEKEERSMAECKER ( 1600) & Margaux GERARD (7659 1600)
// @Date : 11 mai 2018

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
	if(stream == NULL){ // Si la fonction fopen a echoue
		return 1; // Erreur
	}
	char* line = (char*)malloc(sizeof(char)*100);
	if(line == NULL){ // Si le malloc renvoie une erreur ou ne s'est pas execute correctement
		return 1; // Erreur
	}
	printf("Succès de la fonction fopen\n");
	int done = 0;
	while(done == 0){
		done = readline(stream,line);
		struct fractal* fra = create_fractal(line); // On cree une fractale avec la ligne qui a ete lue et splitee
		sem_wait(&empty); // On fait attendre la premiere semaphore
		pthread_mutex_lock(&mutex); // On protege la section critique avec un mutex qu'on bloque
        add_buffer(fra); // Section critique : On ajoute la fractale au buffer
		pthread_mutex_unlock(&mutex); // On debloque le mutex
        sem_post(&full);
	}
}

// Threads de calcul
// Le consommateur va prendre les données du producteur (le buffer)
int consommateur (){
	int err;
	struct fractal* frac = (struct fractal*)malloc(sizeof(struct fractal));
    if (frac == NULL){ // Si le malloc renvoie une erreur ou ne s'est pas execute correctement
        return 1; // Erreur
    }
    while (true){
        sem_wait(&full);
        pthread_mutex_lock(&mutex); // On protege la section critique avec un mutex qu'on bloque
        frac = remove_buffer(); // Section critique : on retire la fractale du mutex
        max_fractale(frac); // Section critique : on calcule la valeur moyenne de cette fractale sur tous les pixels et on la compare a la valeur maximale, si elle est plus grande ou egale, on place la fractale dans la pile
        pthread_mutex_unlock(&mutex); // On debloque le mutex
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
		return 1; // Erreur
	}
	return 0; // La fonction s'est executee correctement
}

// Creation d'une fractale sur base d'une ligne
// OK !
struct fractal* create_fractal(*char line){
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

// Ajouter un élément au buffer
// OK !
int add_buffer(struct fractal *frac){
	for(int i=0;i<10;i++){
		if(buffer[i] == NULL){
			buffer[i] = frac; // On ajoute la fractale frac au buffer a l'emplacement i
			return 0; // La fonction s'est executee correctement
		}
	}
	return 1; // Erreur
}

// Retirer un élément du buffer
// OK !
struct fractal* remove_buffer(){
	for(int i=0;i<10;i++){
		if(buffer[i] != NULL){ // Si il y a un element a l'emplacement i du buffer, on retourne cet element qui est une fractale et on met l'emplacement du buffer a NULL
			struct fractal* temp = buffer[i]; // temp est la fractale contenue a l'emplacement i du buffer
			buffer[i] = NULL; // On met l'emplacement i du buffer a NULL
			return temp; // On retourne la fractale qui a ete supprimee du buffer pour pouvoir l'utiliser par la suite
		}
	}
	return NULL;
}

// On trouve la fractale qui a la valeur moyenne la plus grande
void max_fractale(struct fractal *frac){
    moy = calcul_moyenne(frac); // On appelle la fonction qui va calculer la valeur moyenne de la fractale placee en argument
	if(moy>=max){ // Si la moyenne de frac est plus grande que la valeur max
        max = moy;
		frac_max = frac; // On stocke la fractale donc la valeur de la moyenne est maximale
        fract_push(*frac_max, max); // On ajoute cette fractale a la pile pour qu'elle soit affichee par la suite
	}
}

// On calcule la moyenne des valeurs de la fractale en chaque pixel
int calcul_moyenne(struct fractale *frac){
	unsigned long sum;
	unsigned int width = fractal_get_width(frac); // Variable qui contient la largeur de la fractale
	unsigned int height = fractal_get_height(frac); // Variable qui contient la hauteur de la fractale
	for(int i = 0;i<width;i++){ // On parcourt tous les pixels
		for(int j = 0;j<height;j++){
			sum += fractal_compute_value(frac,i,j); // On fait la somme de la valeur de la fractale a chaque pixel
		}
	}
	return sum/(width*height); // Pour retourner la moyenne, on divise la somme des valeurs de la fractale en chaque pixel par le nombre de pixel qui est width*heigth
}

// Fonction qui va afficher toutes les fractales qui sont présentes dans la pile
void frac_affiche(struct noeud * head){
    if (head == NULL){ // On teste si la pile est nulle
        return NULL;
    }
    int i=0;
    char *fname;
    struct noeud * runner = head; // Pointeur qui va parcourir la pile
    while (runner != NULL){
        fname = "BMP_out_i"; // Nom du fichier BMP de sortie
        write_bitmap_sdl(runner->fract, fname); // Application de la fonction donnee dans l'enonce qui permet d'afficher la fractale sur un certain fichier de sortie
        i++;
        runner = runner->next;
    }
}
