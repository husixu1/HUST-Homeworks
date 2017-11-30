#include<stdio.h>
int main(void){
	char a[32]="12345678912345678912.1234567891";
	char b[32]="98765432109876543210.0123456789";
	int add=0,c=1,i;
	char result[33],*r;
	memset(result,0,33);
	for(i=30;i>=0;i--,c=1){
		if(a[i]=='.')
			result[i+c]='.',i--;
		result[i+c]=(char)((a[i]+b[i]+add-2*'0')%10+'0');
		add=(a[i]+b[i]+add-2*'0')/10;
	}
	result[32]='\0';
	for(r=result;*r=='\0';r++);
	printf("%s\n",r);
	return 0;
}
