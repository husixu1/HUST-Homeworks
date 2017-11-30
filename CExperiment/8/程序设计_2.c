#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct node{
	char name[81];
	char comp[101];
	char tel[21];
	char mail[101];
	struct node *next;
}Node;

void read(Node *head);
void insert(Node *head);
void modify(Node *head);
void delet(Node *head);
void showall(Node *head);
void exiting(Node *head);
int main(void){
	Node *head = (Node *)malloc(sizeof(Node));
	(head->name)[0]=0;
	head->next=NULL;
	int choose;
	for(;;){
		printf("\nchoose from the options:\n"
				"1.read from file\n"
				"2.modify\n"
				"3.insert\n"
				"4.delete\n"
				"5.show all\n"
				"6.sort by name\n"
				"7.save and exit\n\n");
		scanf("%d",&choose);
		getchar();
		switch(choose){
		case 1:
			read(head);
			break;
		case 2:
			modify(head);
			break;
		case 3:
			insert(head);
			break;
		case 4:
			delet(head);
			break;
		case 5:
			showall(head);
			break;
		case 6:
			showall(head);
			break;
		case 7:
			exiting(head);
			break;
		}
	}
}
void read(Node *head){
	FILE *data = fopen("data.dat","rb");
	Node *temp = head;
	for(;fread(head,sizeof(Node),1,data)==1;head = head->next)
		head->next = (Node *)malloc(sizeof(Node));
	for(;temp->next!=head;temp = temp->next);
	temp->next=NULL;
	free(head);
	fclose(data);
}
void exiting(Node *head){
	FILE *data = fopen("data.dat","wb");
	for(;head!=NULL;head=head->next)
		fwrite(head,sizeof(Node),1,data);
	fputc(EOF,data);
	fclose(data);
	exit(0);
}
void insert(Node *head){
	Node ins;
	printf("input name, mail, tel, comp:\n");
	scanf("%s %s %s %s",ins.name,ins.mail,ins.tel,ins.comp);

	Node *temp = head;
	for(;head!=NULL && (head->name)[0]<=(ins.name)[0];head = head->next);
	for(;temp->next!=head;temp = temp->next);
	temp->next = (Node *)malloc(sizeof(Node));
	temp = temp->next;
	temp->next = head;
	strcpy(temp->name,ins.name);
	strcpy(temp->mail,ins.mail);
	strcpy(temp->tel,ins.tel);
	strcpy(temp->comp,ins.comp);
}
void modify(Node *head){
	Node modi;
	char name[81];
	printf("input name to modify:\n");
	scanf("%s",name);
	printf("input name, mail, tel, comp:\n");
	scanf("%s %s %s %s",modi.name,modi.mail,modi.tel,modi.comp);
	for(;head!=NULL && strcmp(head->name,name)!=0;head = head->next);
	strcpy(head->name,modi.name);
	strcpy(head->mail,modi.mail);
	strcpy(head->comp,modi.comp);
	strcpy(head->tel,modi.tel);
}
void delet(Node *head){
	char name[81];
	printf("input name to delete:\n");
	scanf("%s",name);

	Node *temp = head;
	for(;head!=NULL && strcmp(head->name,name)!=0;head = head->next);
	for(;temp->next!=head;temp = temp->next);
	temp->next = head->next;
	free(head);
}
void showall(Node *head){
	for(;head!=NULL;head = head->next)
		if((head->name)[0]!=0)
			printf("%s\n%s\n%s\n%s\n\n",head->name,head->mail,head->tel,head->comp);
}
