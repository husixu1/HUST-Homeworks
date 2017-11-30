#include<stdio.h>
#define MAX_BUFF_SIZE 65535

void m_sort_i(int *buff,int len);
void m_sort_d(int *buff,int len);
int main(int argc, char *argv[]){
	int buff[MAX_BUFF_SIZE];
	void (*f)(int *,int);
	f=(argc==1)?m_sort_i:m_sort_d;

	int N;
	scanf("%d",&N);
	for(int i=0;i<N;i++)
		scanf("%d",buff+i);
	f(buff,N);
	for(int i=0;i<N;i++)
		printf("%5d",buff[i]);
	return 0;
}
void m_sort_i(int *buff,int len){
	if(len==1)
		return;
	m_sort_i(buff,len/2);
	m_sort_i(buff+len/2,len-len/2);
	int temp[MAX_BUFF_SIZE],i=0,j=len/2,c=0;
	for(;i<len/2&&j<len;c++)
		temp[c]=(buff[i]<buff[j])?buff[i++]:buff[j++];
	for(;i<len/2;i++,c++)
		temp[c]=buff[i];
	for(;j<len;j++,c++)
		temp[c]=buff[j];
	for(c=0;c<len;c++)
		buff[c]=temp[c];
}
void m_sort_d(int *buff,int len){
	if(len==1)
		return;
	m_sort_d(buff,len/2);
	m_sort_d(buff+len/2,len-len/2);
	int temp[MAX_BUFF_SIZE],i=0,j=len/2,c=0;
	for(;i<len/2&&j<len;c++)
		temp[c]=(buff[i]>buff[j])?buff[i++]:buff[j++];
	for(;i<len/2;i++,c++)
		temp[c]=buff[i];
	for(;j<len;j++,c++)
		temp[c]=buff[j];
	for(c=0;c<len;c++)
		buff[c]=temp[c];
}
