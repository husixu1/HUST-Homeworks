#include <stdio.h>
int main(void){
    unsigned long ipAdress;
    unsigned short first, second, third, fourth;
    puts("Input the IP Address in unsigned long int form:");
    scanf("%lu",&ipAdress);
    first = (ipAdress&0xFF000000)>>24;
    second = (ipAdress&0xFF0000)>>16;
    third = (ipAdress&0xFF00)>>8;
    fourth = ipAdress&0xFF;
    printf("%hu.%hu.%hu.%hu",fourth,third,second,first);
}
