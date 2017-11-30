#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char *strnins(char *, const char *, int n);
int main(void){
	char s[100],t[100];
	int n;
	scanf("%s %s %d",s,t,&n);
	char *out = strnins(s,t,n);
	printf("%s",out);
}
char *strnins(char *s, const char *t, int n){
	char *temp = (char *)malloc(strlen(s)+strlen(t)+1);
	strcpy(temp,s);
	temp[n]='\0';
	strcat(temp,t);
	strcat(temp,s+n);
	return temp;
}
