#include<stdio.h>
#include<math.h>
#define Y 1
#define N 0
int is_Prieme(int n);
int main(void){
	int to_judge,i;
	scanf("%d",&to_judge);
	for(i=2;i<to_judge;i++)
		if(is_Prieme(i) && is_Prieme(to_judge-i)){
			printf("%d=%d+%d\n",to_judge,i,to_judge-i);
			break;
		}
	return 0;
}
int is_Prieme(int n){
	for(int i=2;i<=sqrt(n);i++)
		if(n%i==0)
			return N;
	return Y;
}

