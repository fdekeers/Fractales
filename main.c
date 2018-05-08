// @Titre : Projet Fractales
// @Auteurs : Francois DE KEERSMAEKER (7367 1600) & Margaux GERARD (7659 1600)
// @Date : 11 mai 2018

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fractal.h"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

// Initialisations
int error = 1;
int succes = 0;
double max;
struct fractal *frac_max = NULL;
struct fractal *buffer[10];
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
struct noeud **head = NULL;


/*
 * readline : Fonction qui lit une ligne sur un fichier d'entrée. Elle passe les lignes vides ou commençant par '#'. OK
 *
 * @stream : fichier d'entrée
 * @buf : buffer qui a ete rempli par le producteur
 * @return: 0 si le fichier n'est pas fini, 1 si il est fini
 */

int readline(FILE* stream, char* buf){
    
	int i = 0;
	char temp = fgetc(stream);
    
	// Ligne vide
	while(temp == '\n'){
        temp = fgetc(stream);
	}
    
	// Ligne commençant par '#'
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
		return 1; // Fichier termine
	}
    
	return 0; // Fichier non termine
}


/*
 * create_fractal : Fonction qui cree une fractale sur base d'une ligne.
 *
 * @line : ligne du fichier avec laquelle on va créer une fractale
 * @return: pointeur vers la fractale qui a ete creee
 */

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
 * add_buffer : Fonction qui ajoute une fractale dans le buffer.
 *
 * @frac : représente la fractale à ajouter au buffer
 * @return: 0 si la fractale a ete ajoutee au buffer correctement, 1 sinon
 */

int add_buffer(struct fractal *frac){
    
	for(int i = 0; i < 10; i++){
        
		if(buffer[i] == NULL){
			buffer[i] = frac; // On ajoute la fractale frac au buffer a l'emplacement i
			return 0;
		}
	}
	return 1;
}

/*
 * remove_buffer : Fonction qui retire une fractale buffer et remet la case a NULL.
 *
 * @return: la fractale qui a ete retiree du buffer
 */

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


/*
 * calcul_moyenne : Fonction qui calcule la moyenne de la valeur  d'une fractale sur chaque pixel.
 *
 * @frac : représente la fractale dont la valeur moyenne sur tous les pixels va etre calculee
 * @return: la valeur moyenne de la fractale sur chaque pixel
 */

int calcul_moyenne(struct fractal *frac){
    
	unsigned long sum;
	unsigned int width = fractal_get_width(frac); // Variable qui contient la largeur de la fractale
	unsigned int height = fractal_get_height(frac); // Variable qui contient la hauteur de la fractale
    
    // On parcourt tous les pixels
	for(unsigned int i = 0; i < width; i++){
        
		for(unsigned int j = 0; j < height; j++){
            
			sum += fractal_compute_value(frac,i,j);
		}
	}
	return sum / (width * height);
}

/*
 * max_fractale : Fonction qui compare la moyenne de la valeur  d'une fractale sur chaque pixel a la variable globale max. La/Les fractales avec la valeur moyenne max sont ajoutee a la pile.
 *
 * @frac : représente la fractale dont la valeur moyenne sur tous les pixels va etre comparee a la variable globale max
 * @return: /
 */

void max_fractale(struct fractal *frac){
    
    double moy = calcul_moyenne(frac);
    
	if(moy>=max){
        
        max = moy;
		frac_max = frac; // On stocke la fractale donc la valeur de la moyenne est maximale
        struct noeud * n = createNoeud(frac_max);
        
        if (*head == NULL){
            *head = createNoeud(frac_max); // C'est le premier noeud, on cree la pile
        }
        
        else {
            push(head,n); // On ajoute cette fractale a la pile pour qu'elle soit affichee par la suite
        }
	}
}


/*
 * producteur : va lire le fichier d'entree ligne pas ligne et spliter chaque ligne pour creer une nouvelle fractale. Elle contient les threads de lecture.
 *
 * @filename: fichier d'entree qui va etre lu
 * @return: 0 si la fonction a execute le code avec succes, 1 sinon
 */

void* producteur(char* filename){
    FILE *stream = fopen(filename,"r");
    
    // On teste si la fonction fopen a echoue
	if(stream == NULL){
		return &ERROR;
	}
    
	char* line = (char*)malloc(sizeof(char)*100);
    
    // On teste si le malloc renvoie une erreur ou ne s'est pas execute correctement
	if(line == NULL){
		return &ERROR;
	}
    
	printf("Succès de la fonction fopen\n");
    
	int done = 0;
    
	while(done == 0){
		done = readline(stream,line);
		struct fractal* fra = create_fractal(line);
		sem_wait(&empty); // On fait attendre la premiere semaphore
		pthread_mutex_lock(&mutex); // On protege la section critique avec un mutex qu'on bloque
        // Section critique
        add_buffer(fra);
        // Fin de la section critique
		pthread_mutex_unlock(&mutex); // On debloque le mutex
        sem_post(&full);
	}
    return &SUCCES;
}

/*
 * consommateur : Le consommateur va prendre les données du producteur (le buffer). Il va ensuite calculer la valeur moyenne d'une fractale sur tous les pixels de celle-ci.
 *                Elle fait entrer en jeu des threads de calcul.
 *
 * @return: 0 si la fonction a execute le code avec succes, 1 sinon
 */

int consommateur (){
	struct fractal* frac = (struct fractal*)malloc(sizeof(struct fractal));
    
    // On teste si le malloc renvoie une erreur ou ne s'est pas execute correctement
    if (frac == NULL){
        return 1;
    }
    
    while (1==1){
        sem_wait(&full);
        pthread_mutex_lock(&mutex); // On protege la section critique avec un mutex qu'on bloque
        // Section critique
        frac = remove_buffer();
        max_fractale(frac);
        // Fin de la section critique
        pthread_mutex_unlock(&mutex); // On debloque le mutex
        sem_post(&empty);
		max_fractale(frac);
    }
    return 0;
}


/*
 * max_fractale : Fonction qui affiche les fractales dont la valeur moyenne sur tous les pixels est maximale dans des fichiers de sortie. Le noeud de la pile est libere apres chaque affichage.
 *
 * @head : Tete de la pile
 * @destination : fichier de sortie sur lequel on va afficher une fractale
 * @return: /
 */

void frac_affiche(struct noeud ** head, char* destination){
    
    if (head == NULL){
        return; // On ne peut rien retourner car c'est une fonction void
    }
    
    struct noeud * runner = *head; // Pointeur qui va parcourir la pile
    
    while (*head != NULL){ // Si la pile contient un ou plusieurs éléments
        write_bitmap_sdl((*head)->fract, destination);
        pop(head);
        runner = runner->next;
        
    }
}


/*
 * main : Fonction principale.
 *
 * @argc : représente la longueur du tableau de char argv
 * @argv : pointeur vers un tableau de char qui sont les arguments
 * @return: 0 si la fonction a execute le code avec succes, 1 sinon
 */

int main(int argc, char *argv[]){
    
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
	
	// Threads de lecture
	
	int err = pthread_mutex_init(&(mutex),NULL);
	err = sem_init(&empty,0,10);
	err = sem_init(&full,0,0);
	
	pthread_t threads_lecture[nfichiers];
    
	for(int i = 0;i<nfichiers;i++){
        
		err = pthread_create(&(threads_lecture[i]),NULL,&(producteur),(void*)fichiers[i]);
        
		if(err != 0){
			printf("Erreur lors de la création des threads de lecture\n");
			return 1;
		}
	}
	
	for(int i=0;i<nfichiers;i++){
        
		int *r;
		err = pthread_join(threads_lecture[i],(void**)&r);
        
		if(err != 0){
			printf("Erreur dans la fonction pthread_join\n");
			return 1;
		}
	}
	
	// Threads de calcul
	pthread_t threads_calcul[maxthreads];
    
	for(int i = 0;i<maxthreads;i++){
        
		err = pthread_create(&(threads_calcul[i]),NULL,&((void*)consommateur),NULL);
        
		if(err != 0){
			printf("Erreur lors de la création des threads de calcul\n");
			return 1;
		}
	}
	
	for(int i = 0;i<maxthreads;i++){
        int *r;
		err = pthread_join(threads_calcul[i],(void**)&r);
		free(r);
        
		if(err != 0){
			printf("Erreur dans la fonction pthread_join\n");
			return 1;
		}
	}
	return 0;
}
