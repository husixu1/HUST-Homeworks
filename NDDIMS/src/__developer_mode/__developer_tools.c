/***********************************************************************
*    NDDIMS - Natural Disasters Data and Information Management System
*    Copyright (C) 2016  HuSixu	 Email:husixu1@hotmail.com
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

/** developer tools, don't use it **/
#include "__developer_tools.h"
#include "node_operation.h"
#include "data_structure.h"
#include "file_operation.h"
#include <stdio.h>

void __develpoer_mode_init(void){
	char input;
	Node *temp;
	for(;;){
		printf("---- i=insert, d=delete, s=show_all, f=find, a=save_all, l=load_all----->>> ");
		input	=	getchar();
		getchar();	//	\n
		switch(input){
			case 'i':
				printf("c=classify, e=event, d=donate ----->>> ");
				/* clear the stdin */
				input	=	getchar();
				getchar();
                if(input=='c'){
					node_init(&temp,node_type_classify);
					printf("name  type  ----->>> ");
					scanf("%s %c",name_classify(temp),&type_classify(temp));getchar();
					node_insert(temp);
                }
				else if(input=='e'){
					node_init(&temp,node_type_event);
					printf("serial  time  name  level  loc  dead  house  money----->>> ");
					scanf("%s %s %s %c %s %d %d %f"
							,serial_event(temp)
							,time_event(temp)
							,name_event(temp)
							,&level_event(temp)
							,location_event(temp)
							,&deads_event(temp)
							,&house_destroyed_event(temp)
							,&pencuniary_loss_event(temp));getchar();
					node_insert(temp);
				}
				else if(input=='d'){
					node_init(&temp,node_type_donate);
					printf("serial  time  type  name  amount class----->>> ");
					scanf("%s %s %c %s %f %s"
							,serial_donate(temp)
							,time_donate(temp)
							,&type_donate(temp)
							,name_donate(temp)
							,&amount_donate(temp)
							,classify_name_donate(temp));getchar();
					node_insert(temp);
				}
				break;
			case 'd':
				break;
			case 's':
                show_all();
				break;
			case 'a':
				file_save("hello");
				break;
			case 'l':
				file_load("hello");
		}
	}
}
//void show_sequence(Node *head){
//
//}
void show_all(void){
	Node *temp1	=	head_classify->next;
	Node *temp2;
	Node *temp3;
	for(;temp1!=NULL;temp1=temp1->next){
		show_data(temp1);
		for(temp2=temp1->child;temp2!=NULL;temp2=temp2->next){
			show_data(temp2);
			for(temp3=temp2->child;temp3!=NULL;temp3=temp3->next)
				show_data(temp3);
		}
	}
}
void show_data(Node *node){
	switch(node->node_type){
	case node_type_classify:
		printf("name:%s\ntype:%c\n",name_classify(node),type_classify(node));
		break;
	case node_type_event:
		printf(	"	serial:%s\n"
				"	time  :%s\n"
				"	name  :%s\n"
				"	level :%c\n"
				"	loc   :%s\n"
				"	deads :%d\n"
				"	house :%d\n"
				" 	money :%f\n"
				,serial_event(node)
				,time_event(node)
				,name_event(node)
				,level_event(node)
				,location_event(node)
				,deads_event(node)
				,house_destroyed_event(node)
				,pencuniary_loss_event(node));
		break;
	case node_type_donate:
		printf(	"		serial:%s\n"
				"		time  :%s\n"
				"		type  :%c\n"
				"		name  :%s\n"
				"		amount:%f\n"
				,serial_donate(node)
				,time_donate(node)
				,type_donate(node)
				,name_donate(node)
				,amount_donate(node));
		break;
	}
}

void p_error(error_t error){
	switch(error){
		case no_error:
			return;
		case error_has_child:
			printf("node has child\n");
			return;
		case error_initialized:
			printf("node already initialized\n");
			return;
		case error_malloc:
			printf("malloc failed\n");
			return;
		case error_not_found:
			printf("node not found\n");
			return;
		case error_no_parent:
			printf("node has no parent\n");
			return;
		case error_parameter:
			printf("parameter fault\n");
			return;
		default:
			printf("unknown error\n");
			return;
	}
}
