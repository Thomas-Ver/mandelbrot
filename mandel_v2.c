#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#define STRMAX 256
#define LONGEUR 1000
#define LARGEUR 1000
#include <string.h>

struct coords2d {
    float x ; /* Cette structure a 2 champs, x et y. Ils sont de type float */
    float y ;
} ;

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
};

double map(int v, int imin, int imax, double omin, double omax){
    return(((omax-omin)/(imax-imin))*v + omax - ((omax-omin)/(imax-imin))*imax);
}

double norme(struct coords2d a){
    return(sqrt(a.x*a.x + a.y*a.y));
}

int main() {

    FILE *fout;
    int i,j,s;
    struct coords2d p;
    struct coords2d c;
    struct coords2d pcopi;
    char ok;
    struct render set;
    int cinter = 0;
    set.minx = -2;
    set.maxx = 0.48;
    set.miny = -1.24;
    set.maxy = 1.24;
    set.L = LARGEUR;
    set.l = LONGEUR;
    set.iter = 90;
    set.ray = 2;
    strcpy(set.basename,"mandeltest1.pbm");
    fout=fopen(set.basename,"a");
    if ( fout == NULL ) {
            ok = 0 ;
            printf("Impossible d'ouvrir le fichier %s\n", set.basename) ;
            exit(1); 
        } 
    else {
            ok = 1 ;
            printf("on a le fichier %s\n", set.basename) ;
        }
    fprintf(fout,"P2\n%d %d\n%d\n#nombre d'itération = %d\n#zone du plan complexe : x [%f,%f] , y [%f,%f]\n",set.l,set.L,set.iter,set.iter,set.minx,set.maxx,set.miny,set.maxy);
    s=0;
    for (i=0;i<set.L;i++){
        for (j=0;j<set.l;j++){
            cinter=0;
            p.x = 0;
            p.y = 0;
            c.x = map(j,0,set.l,set.minx,set.maxx);
            c.y = map(i,0,set.L,set.miny,set.maxy);
            while(cinter<set.iter && norme(p)<2){
                cinter++;
                pcopi.x=p.x;
                pcopi.y=p.y;
                p.x = pcopi.x*pcopi.x - pcopi.y*pcopi.y + c.x ;
                p.y =  2*pcopi.x*pcopi.y + c.y;
            }
           set.img[s]=cinter;
            s++;
        }
    }
    s=0;
    for (i=0;i<set.l;i++){
        fprintf(fout,"\n");
        for (j=0;j<set.L;j++){
            fprintf(fout,"%d \n",set.img[s]);
            s++;
        }
    }
    fclose(fout);
    
    
    return(0);
}

