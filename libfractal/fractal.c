#include <stdlib.h>
#include "fractal.h"

// Structure pour les fractales
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

// Structure pour un noeud de la pile
typedef struct noeud {
    struct node *next;
    int value;
} node;

// Implementation des fonctions pour la pile
// Creation d'un nouveau noeud avec comme valeur value.
// Repris d'Inginious
int fract_push(struct noeud **head, const char *val){
    if (head==NULL) {
        return 1;
    }
    if (*head==NULL) {
        return 1;
    }
    struct node *newnode = (struct node *) malloc (sizeof (struct node));
    if (newnode==NULL) {
        return 1;
    }
    newnode->name = (char *)malloc((strlen(value)+1)*(sizeof (char)));
    if ((newnode->name)==NULL) {
        return 1;
    }
    strcpy(newnode->name,value);
    newnode->next =(*head);
    *head = newnode;
    return 0;
}

// On ajoute un noeud dans la liste
// Repris d'Inginious
int fract_pop(struct noeud **head, char *result){
    if (head==NULL) {
        return 1;
    }
    if (*head==NULL) {
        return 1;
    }
    struct node * newnode = (*head);
    if (newnode->next == NULL){
        free(newnode->name);
        free(newnode);
        return 0;
    }
    strcpy(result,newnode->name);
    (*head)=newnode->next;
    free(newnode->name);
    free(newnode);
    return 0;
}

