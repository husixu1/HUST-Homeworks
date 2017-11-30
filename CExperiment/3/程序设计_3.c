#include<stdio.h>
#include<math.h>
#define Y 1
#define N 0

int is_Prieme(int n);
int main(void){
	int to_judge,i,BEGIN,END;
	for(;;){
		scanf("%d %d",&BEGIN,&END);
		if(BEGIN==0)
			return 0;
		for(int to_judge=BEGIN%2?BEGIN+1:BEGIN;to_judge<=END;to_judge+=2){
			for(i=2;i<to_judge;i++)
				if(is_Prieme(i) && is_Prieme(to_judge-i)){
					printf("%d=%d+%d\n",to_judge,i,to_judge-i);
					break;
				}
		}
		putchar('\n');
	}
	return 0;
}
int is_Prieme(int n){
	for(int i=2;i<=sqrt(n);i++)
		if(n%i==0)
			return N;
	return Y;
}
