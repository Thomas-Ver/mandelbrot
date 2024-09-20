#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct coords2d {
    float x ; /* Cette structure a 2 champs, x et y. Ils sont de type float */
    float y ;
} ;

double map(int v, int imin, int imax, double omin, double omax){
    return(((omax-omin)/(imax-imin))*v + omax - ((omax-omin)/(imax-imin))*imax);
}

double norme(struct coords2d a){
    return(sqrt(a.x*a.x + a.y*a.y));
}

int main() {

    FILE *fout;
    int img[1000*1000];
    char fout_name[1024]="mandel.pbm";
    int cinter; /*compteur d'itération*/
    int i,j,s;
    struct coords2d p;
    struct coords2d c;
    struct coords2d pcopi;
    int WIDHT=1000; /*largeur et hauteur*/
    int HEIGHT=1000;
    char ok;
    fout=fopen(fout_name,"a");
    if ( fout == NULL ) {         /* on vérifie qu'il n'y est pas 'erreur quand on créer le fichier mandel.pbm*/
            ok = 0 ;
            printf("Impossible d'ouvrir le fichier %s\n", fout_name) ;
            exit(1); 
        } 
    else {
            ok = 1 ;
            printf("on a le fichier %s\n", fout_name) ;
        }
    fprintf(fout,"P2\n%d %d\n90\n#nombre d'itération = 90\n#zone du plan complexe : x [-2,0.48] , y [-1.24,1.24]\n",WIDHT,HEIGHT);  /* j'ai utilisé le mode P2 car on peut avoir des images encore plus jolies (on va donner les valuers de 0 a 90 au pixel qui va correspondre a notre nombre d'itérations(expliqué plus bas)!*/
    s=0;
    for (i=0;i<HEIGHT;i++){
        for (j=0;j<WIDHT;j++){
            cinter=0;
            p.x = 0;
            p.y = 0;
            c.x = map(j,0,WIDHT,-2.0,0.48);
            c.y = map(i,0,HEIGHT,-1.24,1.24);
            while(cinter<90 && norme(p)<2){
                cinter++;
                pcopi.x=p.x;
                pcopi.y=p.y;
                p.x = pcopi.x*pcopi.x - pcopi.y*pcopi.y + c.x ;
                p.y =  2*pcopi.x*pcopi.y + c.y;
            }
           img[s]=cinter;   /*enfaite ici notre valeur de jusqu'a ou on est allé dans l'itérations va determiné si le point appartient a lensemble de mandelbrot et sur les bords ca va etre jolie car on va voir la vitesse de convergence des points sur le bord!!!*/
            s++;
        }
    }
    s=0;
    for (i=0;i<HEIGHT;i++){
        fprintf(fout,"\n");
        for (j=0;j<WIDHT;j++){
            fprintf(fout,"%d \n",img[s]);
            s++;
        }
    }
    fclose(fout);
    
    
    return(0);
}

