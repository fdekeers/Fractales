// @Titre : Projet Fractales
// @Auteurs : Francois DEKEERSMAECKER (7367 1600) & Margaux GERARD (7659 1600)
// @Date : 11 mai 2018

#include <stdlib.h>
#include "fractal.h"

// Implementation pour les fractales
struct fractal *fractal_new(const char *name, int width, int height, double a, double b)
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
    return fr;
}

// Fonctions qui concernent les fractales
void fractal_free(struct fractal *f)
{
    free(f);
}

const char *fractal_get_name(const struct fractal *f)
{
    return f->name;
}

int fractal_get_value(const struct fractal *f, int x, int y)
{
    return fractal_compute_value(f,x,y);
}

void fractal_set_value(struct fractal *f, int x, int y, int val)
{
    f->values[x][y]=val;
}

int fractal_get_width(const struct fractal *f)
{
    return f->width;
}

int fractal_get_height(const struct fractal *f)
{
    return f->height;
}

double fractal_get_a(const struct fractal *f)
{
    return f->a;
}

double fractal_get_b(const struct fractal *f)
{
    return f->b;
}

*noeud createNoeud(fractal * f){
    *noeud n = malloc(sizeof(noeud));
    n->fractal = f;
    n->node = NULL;
    return n;
}

void freeNoeud(noeud *n){
    free(n->fractal);
    free(n);
}

// Implementation des fonctions pour la pile

// On ajoute un nouveau noeud a la tete de la pile
// Repris d'Inginious
int push(struct noeud **head, struct noeud *n){
    if (head==NULL) {
        return 1;
    }
    if (*head==NULL) {
        return 1;
    }
    if (newnode==NULL) {
        return 1;
    }
    if (n==NULL) {
        return 1;
    }
    if (n->fractal == NULL){
        return 1;
    }
    n->next = *head;
    *head = n;
    return 0;
}

// On retire le noeud present a la tete de la pile
// Repris d'Inginious
int pop(struct noeud **head, struct fractal *fract){
    if (head==NULL) {
        return 1;
    }
    if (*head==NULL) {
        return 1;
    }
    struct noeud * n = (*head);
    if (n->next == NULL){
        freeNoeud(n);
        return 0;
    }
    (*head)=n->next;
    freeNoeud(n);
    return 0;
}

