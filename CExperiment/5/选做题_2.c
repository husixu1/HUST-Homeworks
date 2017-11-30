#include<stdio.h>
#include<stdlib.h>

int counter=0;
void search(int (*)[8],int );
int can_place(int (*)[8], int, int);

int main(void){
	int board[8];
	search(&board,0);
}
void search(int (*board)[8],int row){
	if(row==8){			//reach the last line
			printf("%d:\n",++counter);
		int i,j;
		for(i=0;i<8;i++){
			for(j=0;j<(*board)[i];j++)
				putchar('.');
			putchar('Q');
			for(;j<8;j++)
				putchar('.');
			putchar('\n');
		}
		return;
	}
	int col=0;
	for(;col<8;col++){
		if(can_place(board,row,col)){
			(*board)[row]=col;
			search(board,row+1);
		}
	}
	if(col==8)
		return;
}
int can_place(int (*board)[8], int row, int col){
	int x,y;
	for(int i=0;i<row;i++)
		if((*board)[i]==col || abs(i-row)==abs((*board)[i]-col))
			return 0;
	return 1;
}
