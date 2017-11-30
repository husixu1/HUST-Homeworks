#include<stdio.h>
void strncat(char (*)[],char (*)[],int);
int main(void)
{
      char a[50]="The adopted symbol is ",b[27]="abcdefghijklmnopqrstuvwxyz";
      strncat(&a, &b, 4);
printf("%s\n",a);
      return 0;
}
void strncat(char (*s)[],char (*t)[], int n)
{
      int i = 0, j;
while((*s)[i++]) ;
i--;
for(j = 0; j < n && (*t)[j];)
(*s)[i++] = (*t)[j++];
      (*s)[i] = '\0';
}
