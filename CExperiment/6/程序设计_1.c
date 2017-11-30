#include<stdio.h>

int main(void){
    long int a;
    int N;
    scanf("%d",&N);
    for(int i=0;i<N;i++){
		i?putchar('\n'):0;
		scanf("%ld",&a);
		for(int i=0;i<8;i++){
			i?putchar(' '):0;
			printf("%X",(unsigned)(a&(0x0f<<(28-4*i)))>>(28-4*i));
		}
	}
	return 0;
}
