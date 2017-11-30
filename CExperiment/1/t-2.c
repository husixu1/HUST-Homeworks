#include <stdio.h>
int main(void){
    unsigned short x,m,n;
    puts("Please input x,m and n, separated by space:");
    scanf("%hu%hu%hu",&x,&m,&n);
    x>>=m;
    x<<=(16 - n);
    printf("%hx\n",x);
    return 0;
}
