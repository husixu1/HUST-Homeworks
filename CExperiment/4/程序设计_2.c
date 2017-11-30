#include<stdio.h>
#define MAX_STR_LEN 65535
int main(void){
	char buff[MAX_STR_LEN];
	for(;fgets(buff,MAX_STR_LEN-1,stdin)!=NULL;){
    	if(buff[0]%2){
    		for(int i=0; buff[i]!='\0'; i++)
    			putchar(('a'<=buff[i]&&buff[i]<='z'||'A'<=buff[i]&&buff[i]<='Z')?\
    					('a'<=buff[i]&&buff[i]<='z')?buff[i]-'a'+'A':buff[i]-'A'+'a':\
    					buff[i]);
    	}
    	else
    		printf("%s",buff);
	}
	return 0;
}
