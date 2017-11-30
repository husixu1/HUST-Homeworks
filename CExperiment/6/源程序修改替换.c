#include<stdio.h>
#include <string.h>
int main(void)
{
	char *(*p)(char *,const char *);
      char a[80],b[80],c[160],*result=c;
	  int choice,i;
	  do{
		  printf("\t\t1 copy string.\n");
		  printf("\t\t2 connect string.\n");
		  printf("\t\t3 exit.\n");
		  printf("\t\tinput a number (1-3) please!\n");
		  scanf("%d",&choice);
	  }while(choice<1 || choice>5);
	  switch(choice){
	  case 1:
		  p=strcpy;
		  break;
	  case 2:
		  p=strcat;
		  break;
	  case 3:
		  goto down;
}
	  getchar();
	  printf("input the first string please!\n");
	  i=0;
		gets(a);
		printf("input the second string please!\n");
	  i=0;
		gets(b);
	  result = p(a,b);
	  printf("the result is %s\n",result);
down:
      return 0;
}
