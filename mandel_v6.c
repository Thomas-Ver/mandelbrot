#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STRMAX 256

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
    int *img;
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

int main(int argc, char *argv[]) { /*argument de main x et y de centre puis la hauteur et longeur de l'image*/

    struct render set;
    struct camera pov;
    char * endPtr;

    pov.x = strtod(argv[2],&endPtr);
    assert( argv[2] != endPtr ); /*verification pour pas de bug*/
    argv[2] = endPtr;
    argv[2][0] = ' ';  /* ici on remplace la virgule par un espace pour que strtod lise bien un floatant pour y*/
    pov.y = strtod(argv[2],&endPtr);
    argv[2] = endPtr;
    argv[2][0] = ' '; 
    pov.width = strtod(argv[2],&endPtr);
    argv[2] = endPtr;
    argv[2][0] = ' ';
    pov.height = strtod(argv[2],&endPtr);

    render_init(&set,argv);
    cam2rect(&set,&pov);
    render_image(&set);
    render_save_altern(&set,argv);
    free(set.img); /*on libére la place utilisé par notre matric image*/
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
    set->l = strtol(argv[3],&endPtr,0);
    argv[3] = endPtr;
    argv[3][0] = ' '; /* on transforme le * en espace pour lire la deuxieme valeur celle de la longeur car on a "longeur*largeur"*/
    set->L = strtol(argv[3],&endPtr,0);
    set->ray = 2;
    set->img = malloc(sizeof(int)*set->l*set->L);
    if ( set->img == NULL ) {
        printf("Erreur d'allocation mémoire !\n") ;
        exit(1) ;
        }

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
    for (i=0;i<set->L;i++){
        fprintf(fout,"\n");
        for (j=0;j<set->l;j++){
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

int m2v(struct render *set, int x, int y){ /*j'en ai pas besoin vue comment j'ai configurer mon image avec mes compteurs s dans les fonctions render_image et render_save_altern*/
    return(x + y*set->l);  /*on compte a partir du bas et de gauche a droite puis on monte tous les y de une longeur dans la matrice image*/

}


