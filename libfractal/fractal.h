// @Titre : Projet Fractales
// @Auteurs : Francois DEKEERSMAECKER ( 1600) & Margaux GERARD (7659 1600)
// @Date : 11 mai 2018

#ifndef _FRACTAL_H
#define _FRACTAL_H

// Structure pour les fractales
struct fractal {
    char *name;
	unsigned int width;
	unsigned int height;
	double a; //partie réelle des coordonnées de la fractale
	double b; //partie imaginaire des coordonnées de la fractale
    int ** values;
};

// Fonctions a implementer pour les fracatles
/*
 * fractal_new: alloue une nouvelle structure fractal
 *
 * @name: nom de la fractale
 * @width: largeur de l'image finale
 * @height: hauteur de l'image finale
 * @a: partie réelle des coordonnées de la fractale
 * @b: partie imaginaire des coordonnées de la fractale
 * @return: un pointeur vers une struct fractal, ou NULL si erreur
 */
struct fractal *fractal_new(const char *name, int width, int height, double a, double b);

/*
 * fractal_free: libère la mémoire utilisée par une struct fractal
 *
 * @f: fractale à libérer
 */
void fractal_free(struct fractal *f);

/*
 * fractal_get_name: retourne le nom de la fractale
 *
 * @f: fractale
 * @return: nom de la fractale
 */
const char *fractal_get_name(const struct fractal *f);

/*
 * fractal_get_value: retourne la valeur correspondant à un pixel de l'image
 *
 * @f: fractale
 * @x: abscisse
 * @y: ordonnée
 * @return: valeur du pixel (x,y) de l'image de la fractale
 */
int fractal_get_value(const struct fractal *f, int x, int y);

/*
 * fractal_set_value: défini la valeur correspondant à un pixel de l'image
 *
 * @f: fractale
 * @x: abscisse
 * @y: ordonnée
 * @val: valeur
 */
void fractal_set_value(struct fractal *f, int x, int y, int val);

/*
 * fractal_get_width: retourne la largeur de l'image de la fractale
 *
 * @f: fractale
 * @return: largeur
 */
int fractal_get_width(const struct fractal *f);

/*
 * fractal_get_height: retourne la hauteur de l'image de la fractale
 * 
 * @f: fractale
 * @return: hauteur
 */
int fractal_get_height(const struct fractal *f);

/*
 * fractal_get_a: retourne la partie réelle des coordonnées de la fractale
 * 
 * @f: fractale
 * @return: partie réelle
 */
double fractal_get_a(const struct fractal *f);

/*
 * fractal_get_b: retourne la partie imaginaire des coordonnées de la fractale
 *
 * @f: fractale
 * @return: partie imaginaire
 */
double fractal_get_b(const struct fractal *f);

/*
 * fractal_compute_value
 *
 * Applique la valeur de récurrence sur la fractale, aux coordonnées correspondantes
 * au pixel (x,y) de l'image finale, et appelle fractal_set_value() pour
 * enregistrer le nombre d'itérations effectuées.
 *
 * @f: fractale
 * @x: abscisse
 * @y: ordonnée
 * @return: nombre d'itérations
 */
int fractal_compute_value(struct fractal *f, int x, int y);

/*
 * write_bitmap_sdl
 *
 * Transforme une fractale en un fichier bitmap (BMP)
 *
 * @f: fractale à transformer
 * @fname: nom du fichier de sortie
 * @return: 0 si pas d'erreurs, -1 sinon
 */
int write_bitmap_sdl(const struct fractal *f, const char *fname);

// Structure pour les noeuds de la pile
typedef struct noeud {
    struct node *next;
    struct fractal fract;
    int value; // Necessaire ?
} node;

// Fonctions a implementer pour la pile

// Creation d'un nouveau noeud avec a l'interieur la fractale fract. On ajoute ce noeud a la tete de la pile
int push(struct noeud **head, struct fractal *fract);

// On retire le noeud qui est a la tete de la pile
int pop(struct noeud **head, struct fractal *fract);


#endif
