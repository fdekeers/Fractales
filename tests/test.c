#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
	printf("argc = %i\n",argc);
	int nfichiers = argc-2;
	int d = 0;
	int maxthreads;
	char* buffer = (char*)malloc(sizeof(char)*20);
	if(buffer == NULL){
		return 1;
	}
	
	
	for(int a = 1;a<argc-1;a++){
		printf("Argument %i : %s\n",a,argv[a]);
	}
	
	// Prise en compte des arguments de la main
	for(int i = 1;i<argc;i++){
		if(strcmp(argv[i],"-d") == 0){
			d = 1;
			nfichiers = nfichiers-1;
			printf("Décrémentation du nombre de fichiers d'entrée\n");
		}
		if(strcmp(argv[i],"--maxthreads") == 0){
			maxthreads = atoi(argv[i+1]);
			nfichiers = nfichiers-2;
		}
	}
	printf("Nombre de fichiers de sortie : %i\n",nfichiers);
	
	
	
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
	
	for(int k = 0;k<nfichiers;k++){
		printf("Fichier %i : %s\n",k+1,fichiers[k]);
	}
}