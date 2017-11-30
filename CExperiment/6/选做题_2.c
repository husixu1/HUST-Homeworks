#include<stdio.h>
#include<string.h>

int main(void){
	int c;
	char str[2][65535];
	char *(*p[2])(const char *,const char *);
	p[0]=strcat,p[1]=strstr;
	printf("Please input function\n1 for strcat\n2 for strstr\n0 to exit\n\n");
	for(;;){
		scanf("%d",&c);
		getchar();
		switch(c){
			case 1:
				printf("please input str1: \n");
				gets(str[1]);
				printf("please input str2: \n");
				gets(str[2]);
				printf("After string catenation, the result is: \n%s\n\n",p[0](str[1],str[2]));
				break;
			case 2:
				printf("please input str1: \n");
				gets(str[1]);
				printf("please input str2: \n");
				gets(str[2]);
				printf("After substring search, the result is: \n%s\n\n",p[1](str[1],str[2])==NULL?"not found!":"found!");
				break;
			case 0:
				return 0;
		}
	}
}
