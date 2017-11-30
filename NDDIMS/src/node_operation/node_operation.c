/***********************************************************************
*	 NDDIMS - Natural Disasters Data and Information Management System
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

#include "data_structure.h"
#include "node_operation.h"
#include <string.h>
#include <stdlib.h>

/* common head */
Node *head_classify;

/* sequence head */
Node *head_cri[MAX_SEQUENCE_TYPE];
/* alias */
Node *head_classify_name;

Node *head_event_serial;
Node *head_event_name;
Node *head_event_time;
Node *head_event_level;
Node *head_event_loc;
Node *head_event_deads;
Node *head_event_house;
Node *head_event_money;

Node *head_donate_name;
Node *head_donate_type;
Node *head_donate_amount;
Node *head_donate_serial;
Node *head_donate_time;

/* inquiring result */
unsigned long	MAX_RESULT_NUM	=	9;
Node 	**inquire_result;

/* statistic result */
Node *head_statistic_type;
Node *head_statistic_name;

error_t head_init(void){
	node_init(&head_classify,node_type_classify);
	node_init(&head_cri[0],node_type_classify);
	node_init(&head_statistic_type,node_type_s_type);
	node_init(&head_statistic_name,node_type_s_name);
	for(int i=1;i<9;i++)
		node_init(&head_cri[i],node_type_event);
	for(int i=1;i<MAX_SEQUENCE_TYPE;i++)
		node_init(&head_cri[i],node_type_donate);
	head_classify_name	=	head_cri[cri_classify_name];
	head_event_serial	=	head_cri[cri_event_serial];
	head_event_name		=	head_cri[cri_event_name];
	head_event_time		=	head_cri[cri_event_time];
	head_event_level	=	head_cri[cri_event_level];
	head_event_loc		=	head_cri[cri_event_loc];
	head_event_deads	=	head_cri[cri_event_deads];
	head_event_house	=	head_cri[cri_event_house];
	head_event_money	=	head_cri[cri_event_money];
	head_donate_name	=	head_cri[cri_event_name];
	head_donate_type	=	head_cri[cri_donate_type];
	head_donate_amount	=	head_cri[cri_donate_amount];
	head_donate_serial	=	head_cri[cri_donate_serial];
	head_donate_time	=	head_cri[cri_donate_time];
	inquire_result	=	(Node **)malloc(sizeof(Node *)*MAX_RESULT_NUM);
	inquire_result[0]=NULL;
	return no_error;
}
error_t node_init(Node **target,node_type_t type){
    *target = (Node *)malloc(sizeof(Node));
    if(target == NULL)
		return error_malloc;

	(*target)->node_type	=	type;
    switch(type){
		case node_type_classify:
			(*target)->data = 	(void *)malloc(sizeof(Classify));
			break;
		case node_type_event:
			(*target)->data	=	(void *)malloc(sizeof(Event));
			break;
		case node_type_donate:
			(*target)->data =	(void *)malloc(sizeof(Donate));
			break;
		case node_type_s_type:
			(*target)->data	=	(void *)malloc(sizeof(S_Type));
			break;
		case node_type_s_name:
			(*target)->data	=	(void *)malloc(sizeof(S_Name));
			break;
    }
	(*target)->prev		=	NULL;
	(*target)->next 		= 	NULL;
	(*target)->child		= 	NULL;
	(*target)->parent		= 	NULL;
	for(int i=0; i<MAX_SEQUENCE_TYPE; i++){
		((*target)->sequence_next)[i]=NULL;
		((*target)->sequence_prev)[i]=NULL;
	}
	return no_error;
}

/* set all the pointer to NULL (used in file_load) */
/* consider converting it to static*/
error_t __node_clear_pointer(Node *target){
	target->prev		=	NULL;
	target->next 		= 	NULL;
	target->child		= 	NULL;
	target->parent		= 	NULL;
	for(int i=0; i<MAX_SEQUENCE_TYPE; i++){
		(target->sequence_next)[i]=NULL;
		(target->sequence_prev)[i]=NULL;
	}
	return no_error;
}

/* insert a node according to a criteria */
#define __cri_insert(criteria,__member)	\
temp 		= 	(head_cri[criteria]->sequence_next)[criteria];\
temp_shadow = 	head_cri[criteria];\
for(;	temp!=NULL &&\
		strcmp(__member(temp),__member(target)) < 0;\
		temp_shadow=temp, temp=(temp->sequence_next)[criteria]);\
(target->sequence_next)[criteria]	=	(temp_shadow->sequence_next)[criteria];\
(target->sequence_prev)[criteria]	=	temp_shadow;\
if((temp_shadow->sequence_next)[criteria]!=NULL)\
	((temp_shadow->sequence_next)[criteria]->sequence_prev)[criteria]=target;\
(temp_shadow->sequence_next)[criteria]	= target;

/* insert a node according to a numeric criteria */
#define __cri_insert_num(criteria,__member) \
temp 		= 	(head_cri[criteria]->sequence_next)[criteria];\
temp_shadow = 	head_cri[criteria];\
for(;	temp!=NULL &&\
		__member(temp) < __member(target);\
		temp_shadow=temp, temp=(temp->sequence_next)[criteria]);\
(target->sequence_next)[criteria]	=	(temp_shadow->sequence_next)[criteria];\
(target->sequence_prev)[criteria]	=	temp_shadow;\
if((temp_shadow->sequence_next)[criteria]!=NULL)\
	((temp_shadow->sequence_next)[criteria]->sequence_prev)[criteria]=target;\
(temp_shadow->sequence_next)[criteria]	= target;

/* insert a node */
error_t node_insert(Node *target){
	Node *temp, *temp_shadow;
    /* insert, at the dame time sort according to every criteria*/
    /* specify the type of the node */
    switch(target->node_type){
		case node_type_classify:
			/* find the duplicate*/
			temp	=	(head_classify_name->sequence_next)[cri_classify_name];
			for(;temp!=NULL;temp=(temp->sequence_next)[cri_classify_name])
				if(strcmp(name_classify(temp),name_classify(target))==0)
					return error_duplicate;

			temp 		=	head_classify->next;
			temp_shadow	=	head_classify;

			/* insert the node */
			target->next		=	temp_shadow->next;
			target->prev		= 	temp_shadow;
			if(temp_shadow->next!=NULL)
				temp_shadow->next->prev	=	target;
			temp_shadow->next	= 	target;

			/* find and insert the node according to name*/;
			__cri_insert(cri_classify_name,name_classify);
			break;

		case node_type_event:
			/* find the duplicate */
			temp	=	(head_event_serial->sequence_next)[cri_event_serial];
			for(;temp!=NULL;temp=(temp->sequence_next)[cri_event_serial])
				if(strcmp(serial_event(temp),serial_event(target))==0)
					return error_duplicate;

			temp = head_classify->next;
			/* find parent node */
			for(; 	temp!=NULL &&
					strcmp(name_classify(temp),name_event(target))!=0;
					temp = temp->next);
			if(temp == NULL)
				return error_no_parent;

			temp_shadow	=	temp;
			temp		=	temp->child;

			/* insert the node */
			target->next	=	temp_shadow->child;
			target->parent	=	temp_shadow;
			if(temp_shadow->child!=NULL)
				temp_shadow->child->prev	=	target;
			temp_shadow->child	=	target;

			/* insert the node according to different criteria */
			__cri_insert(cri_event_name,name_event);
			__cri_insert(cri_event_serial,serial_event);
			__cri_insert(cri_event_time,time_event);
			__cri_insert_num(cri_event_money,pencuniary_loss_event);
			__cri_insert(cri_event_loc,location_event);
			__cri_insert_num(cri_event_level,level_event);
			__cri_insert_num(cri_event_house,house_destroyed_event);
			__cri_insert_num(cri_event_deads,deads_event);
			break;

		case node_type_donate:
			/* find the parent node */
			temp		=	head_event_serial->sequence_next[cri_event_serial];
			temp_shadow	=	head_event_serial;
			for(; 	temp!=NULL && (
				strcmp(serial_event(temp),serial_donate(target))!=0 ||
				strcmp(name_event(temp),classify_name_donate(target))!=0 );
				temp = temp->sequence_next[cri_event_serial]);
			if(temp == NULL)
				return error_no_parent;

			temp_shadow	=	temp;
			temp		=	temp->child;
			/* insert the node */
			target->next		=	temp_shadow->child;
			target->parent		=	temp_shadow;
			if(temp_shadow->child!=NULL)
				temp_shadow->child->prev	=	target;
			temp_shadow->child	=	target;

			/* insert the node according to different criteria */
			__cri_insert_num(cri_donate_amount,amount_donate);
			__cri_insert(cri_donate_name,name_donate);
			__cri_insert_num(cri_donate_type,type_donate);
			__cri_insert(cri_donate_serial,serial_donate);
			__cri_insert_num(cri_donate_time,time_donate);
			break;

		case node_type_s_type:
			temp		=	head_statistic_type->next;
			temp_shadow	=	head_statistic_type;
			/* insert according to type (happening times is changing) */
			for(;temp!=NULL &&
				type_s_type(temp) != type_s_type(target);
				temp_shadow=temp,temp=temp->next);
			/* if find duplicate */
			if(temp!=NULL && type_s_type(temp)==type_s_type(target)){
				times_s_type(temp)			+=1;
				deads_s_type(temp)			+=deads_s_type(target);
				house_destroyed_s_type(temp)+=house_destroyed_s_type(target);
				pecuniary_loss_s_type(temp)	+=pecuniary_loss_s_type(target);
				free(target->data);
				free(target);
			}
			/* if no duplicate*/
			else{
				target->prev		=	temp_shadow;
				target->next		=	temp_shadow->next;
				if(temp_shadow->next!=NULL)
					temp_shadow->next->prev	=	target;
				temp_shadow->next	=	target;
			}
			break;

		case node_type_s_name:
			temp		=	head_statistic_name->next;
			temp_shadow	=	head_statistic_name;
			/* insert according to happening name */
			for(;temp!=NULL &&
				strcmp(name_s_name(temp),name_s_name(target)) > 0;
				temp_shadow=temp,temp=temp->next);
			/* if find the duplicate */
			if(temp!=NULL && strcmp(name_s_name(temp),name_s_name(target))==0){
				times_s_name(temp)			+=	1;
				deads_s_name(temp)			+=	deads_s_name(target);
				house_destroyed_s_name(temp)+=	house_destroyed_s_name(target);
				pecuniary_loss_s_name(temp)	+=	pecuniary_loss_s_name(target);
			}
			else{
				target->prev		=	temp_shadow;
				target->next		=	temp_shadow->next;
				if(temp_shadow->next!=NULL)
					temp_shadow->next->prev	=	target;
				temp_shadow->next	=	target;
				temp				=	target;
			}
			/* copy target to a new */
			node_init(&target,node_type_s_name);
			memcpy(target->data,temp->data,sizeof(S_Name));
			/* insert according to time (second class)*/
			temp_shadow	=	temp;
			temp		=	temp->child;
			/*if has no child */
			if(temp==NULL){
				target->prev		=	NULL;
				target->next		=	NULL;
				target->parent		=	temp_shadow;
				temp_shadow->child	=	target;
			}
			/* if has child*/
			else{
				for(temp_shadow=temp;temp!=NULL	&&
					strcmp(time_s_name(temp),time_s_name(target)) > 0;
					temp_shadow=temp, temp=temp->next);
				if(temp!=NULL && strcmp(time_s_name(temp),time_s_name(target))==0){
					times_s_name(temp)			+=	1;
					deads_s_name(temp)			+=	deads_s_name(target);
					house_destroyed_s_name(temp)+=	house_destroyed_s_name(target);
					pecuniary_loss_s_name(temp)	+=	pecuniary_loss_s_name(target);
				}
				else{
					target->prev		=	temp_shadow;
					target->next		=	temp_shadow->next;
					if(temp_shadow->next!=NULL)
						temp_shadow->next->prev	=	target;
					temp_shadow->next	=	target;
					temp				=	target;
				}
			}

			break;

		/* not any of the type */
		default:
			return	error_parameter;
    }
    return no_error;
}

/* delete a node (has no child) */
error_t node_delete(Node *target){
	if(	target == NULL				||
		target == head_classify		||
		target == head_classify_name||
		target == head_donate_amount||
		target == head_donate_name 	||
		target == head_donate_type 	||
		target == head_event_name 	||
		target == head_event_serial ||
		target == head_event_time)
		return error_parameter;
	if(target->child != NULL)
		return error_has_child;
	/* is classify node */
	if(target->parent == NULL){
		target->prev->next		=	target->next;
		if(target->next!=NULL)
			target->next->prev	=	target->prev;
		free(target->data);
		free(target);
	}
	/* is the first node but not classify node */
	else if(target->parent!=NULL && target->parent->child == target){
		target->parent->child	=	target->next;
        if(target->next!=NULL)
			target->next->prev	=	NULL;
		free(target->data);
		free(target);
	}
	/* not the first node and not the classify node */
	else{
		target->prev->next		=	target->next;
		if(target->next!=NULL)
			target->next->prev 	=	target->prev;
		free(target->data);
		free(target);
	}
	for(int i=0; i<MAX_SEQUENCE_TYPE; i++){
		if((target->sequence_prev)[i] != NULL)
			((target->sequence_prev)[i]->sequence_next)[i] = (target->sequence_next)[i];
		if((target->sequence_next)[i] != NULL)
			((target->sequence_next)[i]->sequence_prev)[i] = (target->sequence_prev)[i];
		free(target->data);
		free(target);
	}
	return no_error;
}
/* recursively delete a node  */
error_t node_delete_recursive(Node *target){
	if(	target == NULL				||
		target == head_classify		||
		target == head_classify_name||
		target == head_donate_amount||
		target == head_donate_name 	||
		target == head_donate_type 	||
		target == head_event_name 	||
		target == head_event_serial ||
		target == head_event_time)
		return error_parameter;
	/* is classify node */
	if(target->parent == NULL){
		Node *temp2,	*temp2_shadow;	/* Event  */
		Node *temp3,	*temp3_shadow;	/* Donate */
		for(temp2=target->child;temp2!=NULL;){
			for(temp3=temp2->child;temp3!=NULL;){
				temp3_shadow	=	temp3;
				temp3			=	temp3->next;
				free(temp3_shadow->data);
				free(temp3_shadow);
			}
			temp2_shadow	=	temp2;
			temp2			=	temp2->next;
			free(temp2_shadow->data);
			free(temp2_shadow);
		}
		target->prev->next		=	target->next;
		if(target->next!=NULL)
			target->next->prev	=	target->prev;
		free(target->data);
		free(target);
	}
	/* is the first node but not classify node */
	else if(target->parent!=NULL && target->parent->child == target){
		/* is the event node */
		if(target->node_type==node_type_event){
			Node *temp3,	*temp3_shadow;	/* donate */
			for(temp3=target->child;temp3!=NULL;){
				temp3_shadow	=	temp3;
				temp3			=	temp3->next;
				free(temp3_shadow->data);
				free(temp3_shadow);
			}
		}
		target->parent->child	=	target->next;
        if(target->next!=NULL)
			target->next->prev	=	NULL;
		free(target->data);
		free(target);
	}
	/* not the first node and not the classify node */
	else{
		/* is the event node */
		if(target->node_type==node_type_event){
			Node *temp3,	*temp3_shadow;	/* donate */
			for(temp3=target->child;temp3!=NULL;){
				temp3_shadow	=	temp3;
				temp3			=	temp3->next;
				free(temp3_shadow->data);
				free(temp3_shadow);
			}
		}
		target->prev->next		=	target->next;
		if(target->next!=NULL)
			target->next->prev 	=	target->prev;
		free(target->data);
		free(target);
	}
	for(int i=0; i<MAX_SEQUENCE_TYPE; i++){
		if((target->sequence_prev)[i] != NULL)
			((target->sequence_prev)[i]->sequence_next)[i] = (target->sequence_next)[i];
		if((target->sequence_next)[i] != NULL)
			((target->sequence_next)[i]->sequence_prev)[i] = (target->sequence_prev)[i];
		free(target->data);
		free(target);
	}
	return no_error;
}

/* delete all node in the chain */
error_t node_delete_all(void){
	Node *temp1,	*temp1_shadow;	/* Classify */
	Node *temp2,	*temp2_shadow;	/* Event	*/
	Node *temp3,	*temp3_shadow;	/* donate	*/
	/* free all nodes */
	for(temp1=head_classify->next;temp1!=NULL;){
		for(temp2=temp1->child;temp2!=NULL;){
			for(temp3=temp2->child;temp3!=NULL;){
				temp3_shadow	=	temp3;
				temp3			=	temp3->next;
				free(temp3_shadow->data);
				free(temp3_shadow);
			}
			temp2_shadow	=	temp2;
			temp2			=	temp2->next;
			free(temp2_shadow->data);
			free(temp2_shadow);
		}
		temp1_shadow	=	temp1;
		temp1			=	temp1->next;
		free(temp1_shadow->data);
		free(temp1_shadow);
	}
	__node_clear_pointer(head_classify);
	__node_clear_pointer(head_statistic_type);
	__node_clear_pointer(head_statistic_name);
	for(int i=0;i<MAX_SEQUENCE_TYPE;i++)
		__node_clear_pointer(head_cri[i]);
	return no_error;
}

#define __cri_inquire(criteria,__member) \
for(;	finder!=NULL &&\
		strcmp(keyword_s,__member(finder))>0;\
		finder=finder->sequence_next[criteria]);\
for(;	finder!=NULL &&\
		strcmp(__member(finder),keyword_e)<0;\
		finder=finder->sequence_next[criteria], counter++){\
		inquire_result[counter]	=	finder;\
		if(counter>=MAX_RESULT_NUM-1){\
			MAX_RESULT_NUM	*=	2;\
			inquire_result	=	realloc(inquire_result,sizeof(Node)*MAX_RESULT_NUM);\
		}\
}

#define __cri_inquire_num(criteria,__member) \
for(;	finder!=NULL &&\
		atof(keyword_s) > __member(finder);\
		finder=finder->sequence_next[criteria]);\
for(;	finder!=NULL &&\
		__member(finder) < atof(keyword_e);\
		finder=finder->sequence_next[criteria], counter++){\
		inquire_result[counter]	=	finder;\
		if(counter>=MAX_RESULT_NUM-1){\
			MAX_RESULT_NUM	*=	2;\
			inquire_result	=	realloc(inquire_result,sizeof(Node)*MAX_RESULT_NUM);\
		}\
}
/* inquire a node according to a criteria (deprecated) */
error_t node_cri_inquire(criteria_t criteria, Node *head, char *keyword_s, char *keyword_e){
	if(keyword_s==NULL || keyword_e==NULL)
		return error_parameter;
	/* make sure keyword_s <= keyword_e */
	if(strcmp(keyword_s,keyword_e)>0){
		char *t;
		t			=	keyword_s;
		keyword_s	=	keyword_e;
		keyword_e	=	t;
	}
	Node *finder	=	head;
	unsigned long counter	=	0;
	switch(criteria){
		case cri_classify_name:
			__cri_inquire(cri_classify_name,name_classify);
			break;
		case cri_event_serial:
			__cri_inquire(cri_event_serial,serial_event);
			break;
		case cri_event_name:
			__cri_inquire(cri_event_name,name_event);
			break;
		case cri_event_time:
			__cri_inquire(cri_event_time,time_event);
			break;
		case cri_event_level:
			__cri_inquire_num(cri_event_level,level_event);
			break;
		case cri_event_loc:
			__cri_inquire(cri_event_loc,location_event);
			break;
		case cri_event_deads:
			__cri_inquire_num(cri_event_deads,deads_event);
			break;
		case cri_event_house:
			__cri_inquire_num(cri_event_house,house_destroyed_event);
			break;
		case cri_event_money:
			__cri_inquire_num(cri_event_money,pencuniary_loss_event);
			break;
		case cri_donate_name:
			__cri_inquire(cri_donate_name,name_donate);
			break;
		case cri_donate_type:
			__cri_inquire_num(cri_donate_type,type_donate);
			break;
		case cri_donate_amount:
			__cri_inquire_num(cri_donate_amount,amount_donate);
			break;
		case cri_donate_serial:
			__cri_inquire(cri_donate_serial,serial_donate);
			break;
		case cri_donate_time:
			__cri_inquire(cri_donate_time,time_donate);
			break;
	}
	inquire_result[counter]=NULL;
	return no_error;
}
//error_t node_vague_inquire(char *keywords, Node *result);
