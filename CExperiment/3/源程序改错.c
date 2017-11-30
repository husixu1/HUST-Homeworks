#include<stdio.h>

long sum_fac(int);
int main(void)
{
        int k;
        scanf("%d",&k);
		printf("k=%d\tthe sum is %ld\n",k,sum_fac(k));
 return 0;
}
long sum_fac(int n)
{
        long s=0;
        int i;
        long fac;
        for(i=1;i<=n;i++){
			fac=1;
			for(int k=1;k<n;k++)
				fac*=k;
			s+=fac;
        }
        return s;
}
