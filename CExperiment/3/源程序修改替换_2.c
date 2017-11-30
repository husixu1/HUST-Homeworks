#include<stdio.h>

float sum_fac(int);
int main(void)
{
        int k;
        scanf("%d",&k);
		printf("k=%d\tthe sum is %f\n",k,sum_fac(k));
 return 0;
}
float sum_fac(int n)
{
        float s=0;
        int i;
        long fac;
        float i_fac;
        for(i=1,fac=1;i<=n;i++)
			fac*=i,s+=(i_fac=1.0/(float)fac);
        return s;
}
