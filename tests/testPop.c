// Code pour tester la fonction pop
// Doit d'abord afficher 'fract1', 'fract2' et 'fract3'
// Puis apres avoir appliquer pop, 'fract1' et 'fract2'

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <fractal.h>
#include <fractal.c>
#include <main.c>

int main() {
    
    char* ligne1 = "fract1 800 800 -0.8 0.4";
    char* ligne2 = "fract2 1000 300 -0.4 0.8";
    char* ligne3 = "fract3 1100 500 -06 0.9";
    
    struct fractal *f = create_fractal(ligne1);
    struct fractal *frac = create_fractal(ligne2);
    struct fractal *fractal = create_fractal(ligne3);
    
    noeud * head = createNoeud(f);
    push(head,frac);
    push(head,fractal);
    
    
    noeud * runner = head;
    noeud * runner2 = head;
    
    while(runner != NULL){
        
        printf(runner->fractal->name);
        runner = runner->next;
        
    }
    
    pop(head);
    
    while(runner2 != NULL){
        
        printf(runner2->fractal->name);
        runner2 = runner2->next;
        
    }
}

