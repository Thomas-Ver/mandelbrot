    #include <stdio.h>
#include <math.h>
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
    int cinter; /*compteur d'itération*/
    int L=50; /*largeur et longuer de l'image*/
    int l=160; 
    int i,j;
    struct coords2d p;
    struct coords2d c;
    struct coords2d pcopi;

    for (i=1;i<L+1;i++){
        printf("\n");
        for (j=1;j<l+1;j++){
            cinter=0;
            p.x = 0;
            p.y = 0;
            c.x = map(j,0,l,-2.0,0.48);
            c.y = map(i,0,L,-1.24,1.24);
           /* printf("%f   %f\n",c.x,c.y);          CA MARCHE ICI EN PLUS*/
            while(cinter<100 && norme(p)<2){
                cinter++;
                pcopi.x=p.x;
                pcopi.y=p.y;
                p.x = pcopi.x*pcopi.x - pcopi.y*pcopi.y + c.x ;
                p.y =  2*pcopi.x*pcopi.y + c.y;
            
            }
            /*printf("%f  %f     ",cx,cy);*/
            /*printf("%f %f      ",p.x,p.y);*/
            if (cinter==100) {
                printf("*");
            }
            else {
                printf(" ");
            }
        }
    }
    printf("\n");
    return(0);
}

