#include<math.h>
#include<stdio.h>
#define S(x,y,z) ((x+y+z)/2.0)
#define AREA(x,y,z) sqrt(S(x,y,z)*(S(x,y,z)-x)*(S(x,y,z)-y)*(S(x,y,z)-z))

int main(void){
    double a,b,c;
        scanf("%lf %lf %lf",&a,&b,&c);
        double x=AREA(a,b,c);
        printf("area=%f\n",AREA(a,b,c));
    return 0;
}
