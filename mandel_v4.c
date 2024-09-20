#include <math.h>
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
    float x;
    float y;
    float width;
    float height;

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
void render_init(struct render *set);
void render_save_bw(struct render *set);
void render_save_altern(struct render *set) ;
void  cam2rect(struct render *set, struct camera *pov);

int main() {

    struct render set;
    struct camera pov;
    pov.x = -1.142817421949293;
    pov.y = -0.21198254168631775;
    pov.width = 0.0055;
    pov.height = 0.0055;

    cam2rect(&set,&pov);
    render_init(&set);
    render_image(&set);
    render_save_altern(&set);
    return(0);
}

void render_save_bw(struct render *set){

    int i,j,s;
    FILE *fout;

    strcpy(set->basename,"mandeltest1.pbm");
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

void render_init(struct render * set){

    set->L = LARGEUR;
    set->l = LONGEUR;
    set->iter = 2000;
    set->ray = 2;

}

void render_save_altern(struct render *set){

    int i,j,s;
    FILE *fout;

    strcpy(set->basename,"mandeltest1.pbm");
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