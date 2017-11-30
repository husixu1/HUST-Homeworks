#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct stu{
	char num[16];
	char name[21];
	float E;
	float M;
	float P;
	float C;
	struct stu* next;
}Stu;
int main(void){
	int N;
	scanf("%d",&N);
	Stu * head = (Stu *)malloc(sizeof(Stu));
	head->num[0]='\0';
	Stu *temp = head;
	for(int i=0;i<N;i++){
		temp->next = (Stu *)malloc(sizeof(Stu));
		temp = temp->next;
		scanf("%s %s %f %f %f %f",(char *)temp->num,(char *)temp->name,&temp->E,&temp->M,&temp->P,&temp->C);
	}

	printf("\nID             Name                English   Math      Physics   C         \n");
	temp = head;
	for(int i=0;i<N;i++){
		temp = temp->next;
		printf("%-15s%-20s%-10.2f%-10.2f%-10.2f%-10.2f\n",temp->num,temp->name,temp->E,temp->M,temp->P,temp->C);
	}

	int M;
	char num[16],sub[20];
	float score;
	scanf("%d",&M);
	for(int i=0;i<M;i++){
		scanf("%s %s %f",(char *)num,(char *)sub,&score);
		for(temp = head->next;strcmp(num,temp->num)!=0;temp = temp->next);
		switch(sub[0]){
		case 'E':
			temp->E = score;
			break;
		case 'M':
			temp->M = score;
			break;
		case 'P':
			temp->P = score;
			break;
		case 'C':
			temp->C = score;
			break;
		}
	}
	printf("\nAlter:\nID             Name                English   Math      Physics   C         \n");
	temp = head;
	for(int i=0;i<N;i++){
		temp = temp->next;
		printf("%-15s%-20s%-10.2f%-10.2f%-10.2f%-10.2f\n",temp->num,temp->name,temp->E,temp->M,temp->P,temp->C);
	}

	printf("\nSumAndAvg:\nID             Name                SUM       AVG       \n");
	temp = head;
	float sum;
	for(int i=0;i<N;i++){
		temp = temp->next;
		sum=(temp->E+temp->M+temp->P+temp->C);
		printf("%-15s%-20s%-10.2f%-10.2f\n",temp->num,temp->name,sum,sum/4.0);
	}

	temp = head;
	Stu* p[N];
	for(int i=0;i<N;i++)
		temp = head->next,p[i] = temp;

	int tt1,tt2;
	for(tt1=0;tt1<N-1;tt1++)
		for(tt2=tt1+1;tt2<N;tt2++)
			if( ((p[tt1])->E+(p[tt1])->M+(p[tt1])->P+(p[tt1])->C) > ((p[tt2])->E+(p[tt2])->M+(p[tt2])->P+(p[tt2])->C) )
				temp = p[tt1];p[tt1]=p[tt2],p[tt2]=temp;

// temp = malloc(sizeof(Stu);
//	for(tt1=0;tt1<N-1;tt1++)
//		for(tt2=tt1+1;tt2<N;tt2++)
//			if( ((p[tt1])->E+(p[tt1])->M+(p[tt1])->P+(p[tt1])->C) > ((p[tt2])->E+(p[tt2])->M+(p[tt2])->P+(p[tt2])->C) ){
//				memcpy(temp,p[tt1],sizeof(Stu));
//				memcpy(p[tt1],p[tt2],sizeof(Stu));
//				memcpy(p[tt2],temp,sizeof(Stu));
//			}

	printf("\nSort:\nID             Name                AVG       \n");
	temp = head;
	for(int i=0;i<N;i++){
		temp = temp->next;
		sum=(temp->E+temp->M+temp->P+temp->C);
		printf("%-15s%-20s%-10.2f\n",temp->num,temp->name,sum/4.0);
	}
	putchar('\n');
	return 0;
}
