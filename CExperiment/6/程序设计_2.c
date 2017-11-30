#include<stdio.h>
#include<stdlib.h>
int main(void){
    int n,i,j,c,c2;
    scanf("%d",&n);
    getchar();
    char *str[n];
	for(i=0;i<n;i++){
		str[i]=(char *)malloc(sizeof(char)*81);
        gets(str[i]);
		for(j=0,c=0;str[i][j+c]!='\0';j++){
			str[i][j]=str[i][j+c];
			if(str[i][j+c]==' '){
				for(;str[i][j+c]==' ';c++);
				c--;
			}
		}
		str[i][j]='\0';
	}
	for(i=0;i<n;i++)
		printf("%s\n",str[i]);
	return 0;
}
