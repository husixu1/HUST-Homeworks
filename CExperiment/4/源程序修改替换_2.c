#include<stdio.h>
#define max(a,b,c) a>(b>c?b:c)?a:(b>c?b:c)

int main(void)
{
      int a, b, c;
      float d, e;
      printf("Enter three integers:");
      scanf("%d %d %d",&a,&b,&c);
      printf("\nthe maximum of them is %d\n",max(a,b,c));
      return 0;
}
