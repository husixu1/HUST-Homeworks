#include<stdio.h>
#include<stdlib.h>

#define N 5
#define M 5

int main(void){
	char ClsNam[5][233];
	char StuNam[5][233];
	scanf("%s %s %s %s %s",(char *)(ClsNam[0]),(char *)(ClsNam[1]),(char *)(ClsNam[2]),(char *)(ClsNam[3]),(char *)(ClsNam[4]));

	int **stu = (int **)malloc(sizeof(int *)*N);
	for(int i=0;i<N;i++)
		*(stu+i) = (int *)malloc(sizeof(int)*M);
	float stuavr[N],clsavr[M];
	int **one=stu;
	float **oneavr;
	int i,j,k;
	float sum;
	for(one=stu;one<stu+N;one++){
		scanf("%s",(char *)StuNam[(one-stu)/sizeof(int**)]);
		for(i=0;i<M;i++)
			scanf("%d",*one+i);
	}
	for(one=stu;one<stu+N;one++){
		for(sum=0,i=0;i<M;i++)
			sum+=*(*one+i);
		printf("Average score of %s is %.2f\n",(char *)StuNam[(one-stu)/sizeof(int**)],*(stuavr+(one-stu)/sizeof(int**))=sum/M);
	}
	for(i=0;i<M;i++){
		for(one=stu,sum=0;one<stu+N;one++)
			sum+=*(*one+i);
		printf("Average score of %s is %.2f\n",(char *)ClsNam[i],*(clsavr+i)=sum/N);
	}
	for(i=0;i<M;i++){
		for(sum=0,one=stu;one<stu+N;one++)
			sum += (*(*one+i)<*(clsavr+i))?1:0;
		printf("Number of students lower than avg of %s is %d\n",(char *)ClsNam[i],(int)sum);
	}
	for(j=0,i=0;i<M;i++){
	 	for(j=0,one=stu;one<stu+N;one++)
	    	j+=(*(*one+i)<60)?1:0;
		printf("Number of students %s fail is %d\n",(char *)ClsNam[i],j);
	}
	for(k=0,i=0;i<M;i++){
	    for(k=0,one=stu;one<stu+N;one++)
		    k+=(*(*one+i)>=90)?1:0;
		printf("Number of students %s perfect is %d\n",(char *)ClsNam[i],k);
	}
}
