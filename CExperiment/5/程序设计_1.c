#include<stdio.h>

int main(void){
	int src[3][4],dst[4][3];
	int i,j;
	for(i=0;i<3;i++)
		for(j=0;j<4;j++)
			scanf("%d",&src[i][j]),dst[j][i]=src[i][j];
	for(i=0;(putchar('\n')),i<3;i++)
		for(j=0;j<4;j++)
			printf("%-4d",src[i][j]);
	for(i=0;(putchar('\n')),i<4;i++)
		for(j=0;j<3;j++)
			printf("%-4d",dst[i][j]);
	return 0;
}
