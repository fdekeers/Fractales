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
#include <SDL.h>

// Initialisations
double max;
pthread_mutex_t mutex_buffer;
pthread_mutex_t mutex_max;
pthread_mutex_t mutex_lecture;
sem_t empty;
sem_t full;
struct fractal *buffer[10] = {NULL};
int bufsize = 10;
int lecture = 0;
struct noeud **head;


/*
 * readline : Fonction qui lit une ligne sur un fichier d'entrée. Elle passe les lignes vides ou commençant par '#'.
 *
 * @stream : fichier d'entrée
 * @buf : buffer qui a ete rempli par le producteur
 * @return: 0 si le fichier n'est pas fini, 1 s'il est fini en lisant une fractale, 2 s'il est fini sans lire de fractale
 *
 */

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


/*
 * create_fractal : Fonction qui cree une fractale sur base d'une ligne.
 *
 * @line : ligne du fichier avec laquelle on va créer une fractale
 * @return: pointeur vers la fractale qui a ete creee
 *
 */

struct fractal* create_fractal(char* line){
    int i = 1;
    // On definit le caractere duquel on va se servir pour split notre ligne
    char* delim = " ";
    char* attr[5];
    // On split la ligne à chaque espace qu'on rencontre
    attr[0] = strtok(line,delim);
    
    while(i<5){
        //  On cree un tableau avec, dans chaque emplacement, un element qui va nous permettre de creer une fractale
        attr[i] = strtok(NULL,delim);
        i++;
    }
    
    // Creation d'une nouvelle fractale
    return fractal_new(attr[0],atoi(attr[1]),atoi(attr[2]),atof(attr[3]),atof(attr[4]));
}

/*
 * add_buffer : Fonction qui ajoute une fractale au buffer.
 *
 * @frac : fractale qui va etre ajoutee au buffer
 * @return: 0 si la fonction a execute le code avec succes, 1 sinon
 *
 */

int add_buffer(struct fractal* frac){
    
    // On parcourt le buffer
    for(int i=0;i<bufsize;i++){
        if(*(buffer+i) == NULL){
            *(buffer+i) = frac;
            return 0;
        }
    }
    return 1;
}

/*
 * remove_buffer : Fonction qui retire une fractale du buffer.
 *
 * @frac : fractale qui va etre supprimee du buffer
 * @return: NULL si une erreur s'est produite, retourne un pointeur vers la fractale qui a ete retiree du buffer
 *
 */

struct fractal* remove_buffer(){
    
    // On parcourt le buffer
    for(int i=0;i<bufsize;i++){
        if(*(buffer+i) != NULL){
            struct fractal* temp = *(buffer+i);
            // On supprime la fractale du buffer
            *(buffer+i) = NULL;
            return temp;
        }
    }
    return NULL;
}

/*
 * buffer_empty : Fonction qui verifie si le buffer est vide.
 *
 * @return: 0 si le buffer n'est pas vide, 1 sinon
 *
 */

int buffer_empty(){
    
    // On parcourt le buffer
    for(int i=0;i<bufsize;i++){
        if(*(buffer+i) != NULL){
            return 0;
        }
    }
    return 1;
}

/*
 * calcul_moyenne : Fonction qui calcule la moyenne de la valeur  d'une fractale sur chaque pixel.
 *
 * @frac : représente la fractale dont la valeur moyenne sur tous les pixels va etre calculee
 * @return: la valeur moyenne de la fractale sur chaque pixel
 */

double calcul_moyenne(struct fractal *frac){
    
    unsigned long r;
    unsigned int width = fractal_get_width(frac); // Variable qui contient la largeur de la fractale
    unsigned int height = fractal_get_height(frac); // Variable qui contient la hauteur de la fractale
    
    // On parcourt tous les pixels
    for(unsigned int i = 0; i < width; i++){
        
        for(unsigned int j = 0; j < height; j++){
            r = r + fractal_compute_value(frac,i,j);
            //printf("r = %li\n",r);
        }
    }
    return ((double)r)/((double)(width*height));
}

/*
 * max_fractale : Fonction qui compare la moyenne de la valeur  d'une fractale sur chaque pixel a la variable globale max. La/Les fractales avec la valeur moyenne max sont ajoutee a la pile.
 *
 * @frac : représente la fractale dont la valeur moyenne sur tous les pixels va etre comparee a la variable globale max
 * @return: /
 */

void max_fractale(struct fractal *frac){
    
    double moy = calcul_moyenne(frac);
    pthread_mutex_lock(&mutex_max);
    
    if(moy>=max){
        max = moy;
        struct noeud * n = createNoeud(frac);
        int err = push(head,n);
    }
    
    pthread_mutex_unlock(&mutex_max);
}

/*
 * producteur : va lire le fichier d'entree ligne pas ligne et spliter chaque ligne pour creer une nouvelle fractale. Elle contient les threads de lecture.
 *
 * @filename: fichier d'entree qui va etre lu
 * @return: succes (0) si la fonction a execute le code avec succes, erreur (1) sinon
 */

void* producteur(char* fname){
    int* error = (int*)malloc(sizeof(int));
    int* success = (int*)malloc(sizeof(int));
    *error = 1;
    *success = 0;
    FILE *stream = fopen(fname,"r");
    
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
            // On fait attendre la premiere semaphore
            sem_wait(&empty);
            // On protege la section critique avec un mutex qu'on bloque
            pthread_mutex_lock(&mutex_buffer);
            
            // Debut de la section critique
            err = add_buffer(fra);
            printf("Fractale ajoutée au buffer\n");
            
            // Affichage du buffer
            for(int i=0;i<bufsize;i++){
                if(*(buffer+i)==NULL){
                    printf("Fractale à la position %i : NULL\n",i+1);
                }
                else{
                    printf("Fractale à la position %i : %s\n",i+1,fractal_get_name(*(buffer+i)));
                }
            }
            
            // Fin de la section critique
            // On debloque le mutex
            pthread_mutex_unlock(&mutex_buffer);
            sem_post(&full);
            i++;
        }
    }
    err = fclose(stream);
    return success;
}

/*
 * consommateur : Le consommateur va prendre les données du producteur (le buffer). Il va ensuite calculer la valeur moyenne d'une fractale sur tous les pixels de celle-ci.
 *                Elle fait entrer en jeu des threads de calcul.
 *
 * @return: succes (0) si la fonction a execute le code avec succes, erreur (1) sinon
 */

void* consommateur (int* d){
    
    int* error = (int*)malloc(sizeof(int));
    *error = 1;
    int val;
    int *sval = &val;
    *error = sem_getvalue(&full,sval);
    
    // Condition sur les threads de calcul
    while(lecture != 0 || *sval != 0){
        
        sem_wait(&full);
        // On protege la section critique avec un mutex qu'on bloque
        pthread_mutex_lock(&mutex_buffer);
        // Debut de la section critique
        struct fractal* frac = remove_buffer();
        printf("Fractale enlevée : %s\n",fractal_get_name(frac));
        // Fin de la section critique
        // On debloque le mutex
        pthread_mutex_unlock(&mutex_buffer);
        sem_post(&empty);
        max_fractale(frac);
        
        if(*d == 1){
            int err = write_bitmap_sdl(frac,fractal_get_name(frac));
        }
        
        *error = sem_getvalue(&full,sval);
    }
    return success;
}

/*
 * main : Fonction principale.
 *
 * @argc : représente la longueur du tableau de char argv
 * @argv : pointeur vers un tableau de char qui sont les arguments
 * @return: 0 si la fonction a execute le code avec succes, 1 sinon
 */

int main(int argc, char *argv[]) {
    
    head = (struct noeud**)malloc(sizeof(struct noeud*));
    *head = (struct noeud*)malloc(sizeof(struct noeud));
    
    int nfichiers = argc-2;
    char* destination = argv[argc-1];
    int dval = 0;
    int *d = &dval;
    int maxthreads = 5;
    
    // Prise en compte des arguments de la main
    for(int i = 1;i<argc;i++){
        
        if(strcmp(argv[i],"-d") == 0){
            *d = 1;
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
    
    // Initialisation des mutex et sémaphores
    int err = pthread_mutex_init(&(mutex_buffer),NULL);
    err = pthread_mutex_init(&(mutex_max),NULL);
    err = pthread_mutex_init(&(mutex_lecture),NULL);
    err = sem_init(&empty,0,bufsize);
    err = sem_init(&full,0,0);
    
    
    // Threads de lecture
    pthread_t threads[nfichiers];
    for(int i = 0;i<nfichiers;i++){
        err = pthread_create(&(threads[i]),NULL,&producteur,fichiers[i]);
        if(err != 0){
            printf("Erreur lors de la création des threads de lecture\n");
            return 1; // Erreur
        }
        // lecture est une varibale globale, on protege sa modification par un mutex pour que 2 threads ne changent pas sa valeur en meme temps
        pthread_mutex_lock(&mutex_lecture);
        lecture++;
        pthread_mutex_unlock(&mutex_lecture);
    }
    
    for(int i=0;i<nfichiers;i++){
        
        err = pthread_join(threads[i],NULL);
        
        if(err != 0){
            printf("Erreur dans la fonction pthread_join\n");
            return 1; // Erreur
        }
        
        // lecture est une varibale globale, on protege sa modification par un mutex pour que 2 threads ne changent pas sa valeur en meme temps
        pthread_mutex_lock(&mutex_lecture);
        lecture--;
        pthread_mutex_unlock(&mutex_lecture);
    }
    
    // Threads de calcul
    pthread_t threads_calcul[maxthreads];
    
    for(int i = 0;i<maxthreads;i++){
        
        err = pthread_create(&(threads_calcul[i]),NULL,&(consommateur),(void*)d);
        
        if(err != 0){
            printf("Erreur lors de la création des threads de calcul\n");
            return 1; // Erreur
        }
    }
    
    for(int i = 0;i<maxthreads;i++){
        
        int *r;
        err = pthread_join(threads_calcul[i],(void**)&r);
        // On libere la memoire
        free(r);
        
        if(err != 0){
            printf("Erreur dans la fonction pthread_join\n");
            return 1; // Erreur
        }
    }
    
    // Destruction des mutex et sémaphores
    pthread_mutex_destroy(&mutex_buffer);
    pthread_mutex_destroy(&mutex_max);
    sem_destroy(&full);
    sem_destroy(&empty);
    
    // Affichage des fractales
    struct noeud* runner = *head;
    int k = 0;
    
    // On parcourt la pile
    while(runner != NULL && runner->fract != NULL){
        
        char* fname = (char*)malloc(sizeof(char)*100);
        
        if(k==0){
            fname = destination;
        }
        
        else{
            err = sprintf(fname,"%s(%i)",destination,k);
        }
        
        // On affiche les noeuds de la pile un par un
        err = write_bitmap_sdl(runner->fract,fname);
        runner = runner->next;
    }
    
    return err;
}
