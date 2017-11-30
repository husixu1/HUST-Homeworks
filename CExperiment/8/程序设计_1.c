#include<stdio.h>
#include<string.h>
int main(void){
	char sentence[65535];
	FILE *outfile;
	outfile = fopen("test","w");
	fgets(sentence,65534,stdin);
	for(int i=0;sentence[i]!='\0';i++){
		if(i==0 && 'a'<=sentence[i] && sentence[i]<='z')
			sentence[i]+=('A'-'a');
		if(sentence[i]==' ' && 'a'<=sentence[i+1] && sentence[i+1]<='z')
			sentence[i+1]+=('A'-'a');
	}
	fwrite(sentence,sizeof(char),strlen(sentence),outfile);
	return 0;
}
