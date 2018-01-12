#include <stdio.h>

int main(void){
	char in_name[10]="";

	do{
		printf("Please input the student's name: ");
		scanf("%[^\n]",in_name);
		getchar();
	}while(!in_name[0]);

	if(in_name[0]=='q' && !in_name[1])
		return 0;

	printf("entering next procedure...\n");
	return 0;
}

