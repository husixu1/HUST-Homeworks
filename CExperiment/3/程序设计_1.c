#include<stdio.h>
int biggest_common_factor(int i,int j);
int main(void){
	int i,j;
	for(;;){
    	scanf("%d %d",&i,&j);
    	if(i==0)
    	    return 0;
    	printf("%d\n",biggest_common_factor(i,j));
    	return 0;
	}
}
int biggest_common_factor(int i, int j){
	if(j<=i)
		if(i%j==0)
			return(j);
		else
			return (biggest_common_factor(j,i%j));
	else
		return(biggest_common_factor(j,i));
}
