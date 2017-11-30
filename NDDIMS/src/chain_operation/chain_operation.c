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

#include "chain_operation.h"
#include "node_operation.h"
#include "data_structure.h"
#include <string.h>
#include <stdlib.h>

/* after each search , clear the search node */
/* search(vague) the classify node according to name */
error_t chain_inquire_classify(const char type,const char *keyword){
    Node *finder;
    unsigned long counter=0;
    for(finder=(head_classify_name->sequence_next)[cri_classify_name];
		finder!=NULL;finder=finder->sequence_next[cri_classify_name]){
		if(	(type	==	'0'		||	type==type_classify(finder)					)
		&&	(keyword	==	NULL||	strstr(name_classify(finder),keyword)!=NULL	)){
			if(counter	>=	MAX_RESULT_NUM-1){
				inquire_result	=	realloc(inquire_result,sizeof(Node *)*MAX_RESULT_NUM*2);
				MAX_RESULT_NUM<<=1;
			}
			inquire_result[counter]	=	finder, counter++;
		}
	}
	inquire_result[counter]=NULL;
	return no_error;
}

error_t chain_inquire_event(
	const char	*serial_start,	const char	*serial_end,
	const char	*name_key,
	const char	*time_start,	const char	*time_end,
	const int	level_start,	const int	level_end,
	const int	deads_start,	const int	deads_end,
	const int	house_start,	const int	house_end,
	const float	money_start,	const float	money_end){

	Node	*finder	=	(head_event_name->sequence_next)[cri_event_name];
	unsigned long counter	=	0;
	for(;finder!=NULL;finder=(finder->sequence_next)[cri_event_name]){
		/* find according ones */
		if(
				(name_key		==NULL	||	strstr(name_event(finder),name_key)!=NULL	)
		&&		(serial_start	==NULL	||	strcmp(serial_start,serial_event(finder))<=0)
		&&		(time_start		==NULL	||	strcmp(time_start,time_event(finder))	 <=0)
		&&		(deads_start	<0		||	deads_start<=deads_event(finder)			)
		&&		(level_start	<0		||	level_start<=level_event(finder)			)
		&&		(house_start	<0		||	house_start<=house_destroyed_event(finder)	)
		&&		(money_start	<0		||	money_start<=pencuniary_loss_event(finder)	)

		&&		(serial_end		==NULL	||	strcmp(serial_end,serial_event(finder))	>=0	)
		&&		(time_end		==NULL	||	strcmp(time_end,time_event(finder))		>=0	)
		&&		(deads_end		<0		||	deads_end>=deads_event(finder)				)
		&&		(level_end		<0		||	level_end>=level_event(finder)				)
		&&		(house_end		<0		||	house_end>=house_destroyed_event(finder)	)
		&&		(money_end		<0		||	money_end>=pencuniary_loss_event(finder)	)){
			if(counter	>=	MAX_RESULT_NUM-1){
				inquire_result	=	realloc(inquire_result,sizeof(Node *)*MAX_RESULT_NUM*2);
				MAX_RESULT_NUM<<=1;
			}
			inquire_result[counter]	=	finder;
			counter++;
		}
	}
	inquire_result[counter]=NULL;
	return no_error;
}

error_t chain_inquire_donate(
	const char	*name_key,
	const char	*serial_start,	const char	*serial_end,
	const char	*time_start,	const char	*time_end,
	const float	amount_start,	const float	amount_end,
	const char	*classify_name,
	const char	type									){

	Node	*finder	=	(head_donate_serial->sequence_next)[cri_donate_serial];
	unsigned long counter	=	0;
	for(;finder!=NULL;finder=finder->sequence_next[cri_donate_serial]){
		/* find according ones */
		if(
				(name_key		==NULL	||	strstr(name_donate(finder),name_key)				!=NULL	)
		&&		(classify_name	==NULL	||	strstr(classify_name_donate(finder),classify_name)	!=NULL	)
		&&		((type!='1'&&type!='2')	||	type==type_donate(finder)									)

		&&		(serial_start	==NULL	||	strcmp(serial_start,serial_donate(finder))			<=0		)
		&&		(time_start		==NULL	||	strcmp(time_start,time_event(finder))				<=0		)
		&&		(amount_start	<0		||	amount_start<=amount_donate(finder)							)

		&&		(serial_end		==NULL	||	strcmp(serial_end,serial_donate(finder))			>=0		)
		&&		(time_end		==NULL	||	strcmp(time_end,time_event(finder))					>=0		)
		&&		(amount_end		<0		||	amount_end>=amount_donate(finder)							)){
			if(counter	>=	MAX_RESULT_NUM-1){
				inquire_result	=	realloc(inquire_result,sizeof(Node *)*MAX_RESULT_NUM*2);
				MAX_RESULT_NUM<<=1;
			}
			inquire_result[counter]	=	finder, counter++;
		}
	}
	inquire_result[counter]=NULL;
	return no_error;
}

error_t chain_inquire_statistic_event(const char type, int level_start, int level_end, const char *name){
    /*clear the previous node */
	Node *temp1,	*temp1_shadow;	/* S_Name && S_Type*/
	Node *temp2,	*temp2_shadow;	/* S_Month		   */
	/* free all nodes */
	for(temp1=head_statistic_name->next;temp1!=NULL;){
		for(temp2=temp1->child;temp2!=NULL;){
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
	for(temp1=head_statistic_type->next;temp1!=NULL;){
		temp1_shadow	=	temp1;
		temp1			=	temp1->next;
		free(temp1_shadow->data);
		free(temp1_shadow);
	}
	__node_clear_pointer(head_statistic_name);
	__node_clear_pointer(head_statistic_type);
	/* find and do statistic */
	Node *finder_classify	=	head_classify->next;
	Node *finder_event;
	/* statistic chain pointer */
	Node *statistic_type;
	Node *statistic_name;

	for(;finder_classify!=NULL;finder_classify=finder_classify->next){
		/* if classify node fit the criteria*/
		if(	(type < '1'		||	type	==	type_classify(finder_classify)		)
		&&	(name == NULL	||	strcmp(name_classify(finder_classify),name)==0	)){
			for(finder_event=finder_classify->child;
			finder_event!=NULL;
			finder_event=finder_event->next){
			/* if event node fit the criteria */
				if(	(level_start<=0	||	level_start	<=level_event(finder_event))
				&&	(level_end	<=0	||	level_end	>=level_event(finder_event))){
					/* do statistics */
					node_init(&statistic_type,node_type_s_type);
					type_s_type(statistic_type)				=	type_classify(finder_event->parent);
					times_s_type(statistic_type)			=	1;
					deads_s_type(statistic_type)			=	deads_event(finder_event);
					house_destroyed_s_type(statistic_type)	=	house_destroyed_event(finder_event);
					pecuniary_loss_s_type(statistic_type)	=	pencuniary_loss_event(finder_event);
					node_insert(statistic_type);

					node_init(&statistic_name,node_type_s_name);
					strcpy(name_s_name(statistic_name),name_event(finder_event));
					strncpy(time_s_name(statistic_name),time_event(finder_event),8);
					time_s_name(statistic_name)[8]	=	'\0';
					times_s_name(statistic_name)			=	1;
					deads_s_name(statistic_name)			=	deads_event(finder_event);
					house_destroyed_s_name(statistic_name)	=	house_destroyed_event(finder_event);
					pecuniary_loss_s_name(statistic_name)	=	pencuniary_loss_event(finder_event);
					node_insert(statistic_name);
				}
			}
		}
	}
	return no_error;
}

