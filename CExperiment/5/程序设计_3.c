#include<stdio.h>
#include<stdlib.h>

#define MAX_STU_NUM	200
#define MAX_NAM_LEN	100

typedef struct{
	int score;
	char name[MAX_NAM_LEN];
}Stu;

void m_sort(Stu *, int);
Stu b_find(Stu *, int, int);

int main(void){
	Stu stu[MAX_STU_NUM];
    int N;
    scanf("%d",&N);			//N students
	for(int i=0; i<N; i++){
		scanf("%s %d",stu[i].name,&stu[i].score);
	}
	m_sort(stu,N);

	putchar('\n');
	for(int i=0;i<N;i++)
		printf("%s %d\n",stu[N-i-1].name,stu[N-i-1].score);

	printf("\nfind:");
	int target;
	scanf("%d",&target);
	Stu	temp = b_find(stu,N,target);
	if(temp.score==-1)
		printf("%s\n","not found!\n");
	else
		printf("%s %d\n",temp.name,temp.score);
}

void m_sort(Stu *begin, int len){
	if(len==1)
		return;
	m_sort(begin,len/2);
	m_sort(&begin[len/2],len-len/2);
	Stu buff[len];
	int i=0,j=len/2,counter=0;
	for(;i<len/2 && j<len;counter++){
		if(begin[i].score<begin[j].score)
			buff[counter]=begin[i],i++;
		else
			buff[counter]=begin[j],j++;
	}
	for(;i<len/2;counter++,i++)
		buff[counter]=begin[i];
	for(;j<len;counter++,j++)
		buff[counter]=begin[j];
	for(counter=0;counter<len;counter++)
		begin[counter]=buff[counter];
	return;
}
Stu b_find(Stu *stu,int len,int target){		//Assume the Stu sequence is already sorted
	if(len==1 && stu[0].score!=target){
		Stu temp = {-1,"not found"};
		return temp;
	}
    if(stu[len/2].score==target)
		return stu[len/2];
	else
		return stu[len/2].score<target?b_find(stu+len/2,len-len/2,target):b_find(stu,len/2,target);
}
