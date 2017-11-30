#include<stdio.h>
#define MAX_STR_LEN	(sizeof(int)<<3)
int main(void){
	int in,i;
	char out[MAX_STR_LEN+1];
	scanf("%d",&in);
	for(i=0;i<MAX_STR_LEN;i++)
		out[i]=((in)&(1<<(MAX_STR_LEN-i-1)))?'1':'0';
	out[i]='\0';
	for(i=0;out[i]=='0';i++);
	printf("%s\n",i==MAX_STR_LEN?"0":&out[i]);
	return 0;
}
