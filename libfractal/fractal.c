// @Titre : Projet Fractales
// @Auteurs : Francois DE KEERSMAEKER (7367 1600) & Margaux GERARD (7659 1600)
// @Date : 11 mai 2018

#include <stdlib.h>
#include "fractal.h"

// Implementation pour les fractales
struct fractal *fractal_new(char *name, int width, int height, double a, double b)
{
	struct fractal *fr = (struct fractal*)malloc(sizeof(struct fractal));
    if (fr==NULL){
        return NULL;
    }
    fr->name = name;
    fr->width = width;
    fr->height = height;
    fr->a = a;
    fr->b = b;
	fr->values = (double**)calloc(width,sizeof(double*));
	for(int i=0;i<width;i++){
		fr->values[i] = (double*)calloc(height,sizeof(double));
	}
    return fr;
}

// Fonctions qui concernent les fractales
void fractal_free(struct fractal *f)
{
    free(f);
}

char *fractal_get_name(struct fractal *f)
{
	if(f == NULL){
		return NULL;
	}
    return f->name;
}

int fractal_get_value(struct fractal *f, int x, int y)
{
    return f->values[y][x];
}


void fractal_set_value(struct fractal *f, int x, int y, int val)
{
    f->values[y][x]=val;
}


int fractal_get_width(struct fractal *f)
{
    return f->width;
}

int fractal_get_height(struct fractal *f)
{
    return f->height;
}

double fractal_get_a(struct fractal *f)
{
    return f->a;
}

double fractal_get_b(struct fractal *f)
{
    return f->b;
}

struct noeud* createNoeud(struct fractal * f){
    struct noeud* n = malloc(sizeof(struct noeud));
    n->fract = f;
    n->next = NULL;
    return n;
}

void freeNoeud(struct noeud *n){
    free(n->fract);
    free(n);
}

// Implementation des fonctions pour la pile

// On ajoute un nouveau noeud a la tete de la pile
// Repris d'Inginious
int push(struct noeud **head, struct noeud *n){
    if (head==NULL) {
        head = (struct noeud**)malloc(sizeof(struct noeud*));
		*head = (struct noeud*)malloc(sizeof(struct noeud));
		*head = n;
		return 0;
    }
    if (*head==NULL) {
        *head = (struct noeud*)malloc(sizeof(struct noeud));
		*head = n;
		return 0;
    }
    if (n==NULL) {
        return 1;
    }
    if (n->fract == NULL){
        return 1;
    }
    n->next = *head;
    *head = n;
    return 0;
}

// On retire le noeud present a la tete de la pile
// Repris d'Inginious
struct fractal* pop(struct noeud **head){
    if (head==NULL) {
        return NULL;
    }
    if (*head==NULL) {
        return NULL;
    }
    struct noeud * n = (*head);
    if (n->next == NULL){
		head = NULL;
        return n->fract;
    }
    (*head)=n->next;
    return n->fract;
}

