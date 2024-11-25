#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>





#define STRMAX 256
#define LONGEUR 1000
#define LARGEUR 1000

struct coords2d {
    float x ; /* Cette structure a 2 champs, x et y. Ils sont de type float */
    float y ;
} ;

struct camera{
    double x;
    double y;
    double width;
    double height;

};

struct render{
    float minx;
    float maxx;
    float miny;
    float maxy;
    int L;
    int l;
    int iter;
    float ray;
    int img[LONGEUR*LARGEUR];
    char basename[STRMAX];
    struct camera pov;
};



double map(int v, int imin, int imax, double omin, double omax){
    return(((omax-omin)/(imax-imin))*v + omax - ((omax-omin)/(imax-imin))*imax);
}

double norme(struct coords2d a){
    return(sqrt(a.x*a.x + a.y*a.y));
}

void render_image(struct render *set);
void render_init(struct render *set, char *argv[]);
/*void render_save_bw(struct render *set); on sen sert plus */
void render_save_altern(struct render *set,  char *argv[]) ;
void  cam2rect(struct render *set, struct camera *pov);
char *my_strdup(const char *s);

int main(int argc, char *argv[]) { /*argument de main x et y de centre puis la hauteur et longeur de l'image*/

    struct render set;
    struct camera pov;


    printf("argc = %d\n",argc);
    printf("argv[1] = %s\n",argv[1]);
    printf("argv[2] = %s\n",argv[2]);

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <param1> <x,y,width,height>\n", argv[0]);
        return EXIT_FAILURE;}


    char *input = my_strdup(argv[2]); // Crée une copie locale modifiable de argv[2]
    if (input == NULL) {
        perror("Failed to allocate memory");
        return EXIT_FAILURE;
    }

    char *endPtr;

    // Remplace les virgules par des espaces pour permettre l'analyse par strtod
    for (char *p = input; *p; ++p) {
        if (*p == ',') *p = ' ';
    }

    // Extraire les valeurs x, y, width et height
    pov.x = strtod(input, &endPtr);
    assert(input != endPtr);  // Vérifie qu'un nombre a bien été extrait

    pov.y = strtod(endPtr, &endPtr);
    assert(endPtr != NULL);   // Vérifie que y a été extrait

    pov.width = strtod(endPtr, &endPtr);
    assert(endPtr != NULL);   // Vérifie que width a été extrait

    pov.height = strtod(endPtr, &endPtr);
    assert(endPtr != NULL);   // Vérifie que height a été extrait

    free(input);  // Libère la mémoire allouée pour la copie locale

    // Affiche les résultats pour vérifier
    printf("x: %f, y: %f, width: %f, height: %f\n", pov.x, pov.y, pov.width, pov.height);

    render_init(&set,argv);
    cam2rect(&set,&pov);
    render_image(&set);
    render_save_altern(&set,argv);
    return(0);
}

void render_save_bw(struct render *set){

    int i,j,s;
    FILE *fout;

    strcpy(set->basename,"mandeltest1");
    fout=fopen(set->basename,"a");
    if ( fout == NULL ) {
            printf("Impossible d'ouvrir le fichier %s\n", set->basename) ;
            exit(1); 
        } 
    else {
            printf("on a le fichier %s\n", set->basename) ;
        }
    fprintf(fout,"P2\n%d %d\n%d\n#nombre d'itération = %d\n#zone du plan complexe : x [%f,%f] , y [%f,%f]\n",set->l,set->L,set->iter,set->iter,set->minx,set->maxx,set->miny,set->maxy);
    s=0;
    for (i=0;i<set->l;i++){
        fprintf(fout,"\n");
        for (j=0;j<set->L;j++){
            fprintf(fout,"%d \n",set->img[s]);
            s++;
        }
    }
    fclose(fout);

}

void render_image(struct render *set){
    
    int i,j,s;
    struct coords2d p;
    struct coords2d c;
    struct coords2d pcopi;
    int cinter = 0;

    s=0;
    for (i=0;i<set->L;i++){
        for (j=0;j<set->l;j++){
            cinter=0;
            p.x = 0;
            p.y = 0;
            c.x = map(j,0,set->l,set->minx,set->maxx);
            c.y = map(i,0,set->L,set->miny,set->maxy);
            while(cinter<set->iter && norme(p)<2){
                cinter++;
                pcopi.x=p.x;
                pcopi.y=p.y;
                p.x = pcopi.x*pcopi.x - pcopi.y*pcopi.y + c.x ;
                p.y =  2*pcopi.x*pcopi.y + c.y;
            }
           set->img[s]=cinter;
            s++;
        }
    }

}

void render_init(struct render *set, char *argv[]){

    char * endPtr;

    set->iter = strtol(argv[1],NULL,0);
    set->L = strtol(argv[3],&endPtr,0);
    argv[3] = endPtr;
    argv[3][0] = ' '; /* on transforme le * en espace pour lire la deuxieme valeur celle de la longeur car on a "largeur*longeur"*/
    set->l = strtol(argv[3],&endPtr,0);
    set->ray = 2;

}

void render_save_altern(struct render *set, char *argv[]){

    int i,j,s;
    FILE *fout;

    strcpy(set->basename,strcat(argv[4],".pbm"))   ; /*ici on concatene .pbm au nom de notre fichier pour créer le fichier dans le mode pbm*/
    fout=fopen(set->basename,"a");
    if ( fout == NULL ) {
            printf("Impossible d'ouvrir le fichier %s\n", set->basename) ;
            exit(1); 
        } 
    else {
            printf("on a le fichier %s\n", set->basename) ;
        }
    fprintf(fout,"P3\n%d %d\n%d\n#nombre d'itération = %d\n#zone du plan complexe : x [%f,%f] , y [%f,%f]\n",set->l,set->L,set->iter,set->iter,set->minx,set->maxx,set->miny,set->maxy); 
    s=0;
    for (i=0;i<set->l;i++){
        fprintf(fout,"\n");
        for (j=0;j<set->L;j++){
            fprintf(fout,"%d %d %d \n",set->img[s],set->img[s],set->img[s]);
            s++;
            
        }
    }
    fclose(fout);

}

void  cam2rect(struct render *set, struct camera *pov){

    set->minx = pov->x - (pov->width)/2;
    set->maxx = pov->x + (pov->width)/2;
    set->miny = pov->y - (pov->height)/2;
    set->maxy = pov->y + (pov->height)/2;
     
}

char *my_strdup(const char *s) {
    char *copy = malloc(strlen(s) + 1); // Alloue suffisamment de mémoire
    if (copy != NULL) {
        strcpy(copy, s); // Copie la chaîne source
    }
    return copy;
}
