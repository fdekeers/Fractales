// Code pour tester la fonction push
// Doit renvoyer le nom des 2 fractales = 'fract1' et 'fract2'

#include <fractal.h>
#include <fractal.c>
#include <main.c>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    
	char* ligne1 = "fract1 800 800 -0.8 0.4";
    char* ligne2 = "fract2 1000 300 -0.4 0.8";
    
    struct fractal *f = create_fractal(ligne1);
    struct fractal *frac = create_fractal(ligne2);
    
    noeud * head = createNoeud(f);
    push(head,frac);
    noeud * runner = head;
    
    while(runner != NULL){
        
        printf(runner->fractal->name);
        runner = runner->next;
        
    }
}
