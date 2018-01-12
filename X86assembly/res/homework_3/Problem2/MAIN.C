#include <stdio.h>
#include <stdlib.h>

//TODO: 2-0 error

extern int registeration(void);
extern int calcavg(void);
extern int calcrank(void);
extern int printall(void);

int main(void){
	int choice, temp;
	for(;;){
		system("cls");
		printf(
			" Welcome to Student Manage Program ////////\n"
			"///////////////////////////////////////////\n"
			"    1) Record Student info                 \n"
			"    2) Calculate Average Score             \n"
			"    3) Calculate Ranks                     \n"
			"    4) Show Grades and Ranks               \n"
			"    0) Exit                                \n"
			"///////////////////////////////////////////\n"
			"Please input your choice >>> ");
		choice = getchar();
		/* flush the input stream */
		while(choice!='\n' && (temp=getchar())!='\r' && temp!='\n');
		switch(choice){
			case '1':
				registeration();
				break;
			case '2':
				calcavg();
				break;
			case '3':
				calcrank();
				break;
			case '4':
				printall();
				break;
			case '0':
			printf(
				"///////////////////////////////////////////\n"
				" Thanks for using this system, bye ////////\n"
				"///////////////////////////////////////////\n");
				return 0;
			default:
				break;
		}
	}
}

int cprint(char *pointer){
	char *temp = pointer;
	for(;*pointer!='$'; pointer++);
	*pointer=0;
	return printf(temp);
}

int cscan(char *pointer, int len){
	char *temp=pointer+2, counter=0;
	int result = scanf("%[^\n]", temp);
	getchar();									// remove the return
	for(;counter<len-1 && *temp != '\0'; counter++, temp++);
	*temp='\r';
	*(pointer+1)=counter;						// store the actually got num
	return result;
}
