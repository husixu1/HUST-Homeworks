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

#ifndef DATA_STRUCTURE_H_INCLUDED
#define DATA_STRUCTURE_H_INCLUDED

/* three kinds of nodes */
typedef char node_type_t;
#define	node_type_classify	0x00
#define node_type_event 	0x01
#define node_type_donate	0x02
/*the statistic result node */
#define node_type_s_type	0x03
#define node_type_s_name	0x04

/* the inquiring and sorting criteria: cri_<node type>_<criteria>*/
typedef int criteria_t;
#define cri_classify_name	0x00

#define cri_event_serial	0x01
#define cri_event_name		0x02
#define cri_event_time		0x03
#define cri_event_level		0x04
#define cri_event_loc		0x05
#define cri_event_deads		0x06
#define cri_event_house		0x07
#define cri_event_money		0x08

#define cri_donate_name		0x09
#define cri_donate_type		0x0A
#define cri_donate_amount	0x0B
#define cri_donate_serial	0x0C
#define cri_donate_time		0x0D

/* biggest sorting criteria types */
#define MAX_SEQUENCE_TYPE	14

/* node operating errors */
typedef char error_t;
#define no_error			0x00
#define error_malloc		0x01
#define error_parameter		0x02

#define error_no_parent		0x03
#define error_has_child		0x04
#define error_initialized	0x05
#define error_not_found		0x06
#define error_duplicate		0x07

/* basic link node */
typedef struct node{
	node_type_t node_type;
	/* Natural Sequence */
	struct node *prev;
	struct node *next;
	/* parent node and child node */
	struct node *parent;
	struct node *child;
	/* inquire sequence, using different criteria*/
	struct node *sequence_prev[MAX_SEQUENCE_TYPE];
	struct node *sequence_next[MAX_SEQUENCE_TYPE];
	/* point to data structure */
	void *data;
	/* mark the temporary save sequence(which line) */
}Node;

#define data_classify(Nodep) 	((Classify *)(Nodep->data))
#define data_event(Nodep)		((Event *)(Nodep->data))
#define data_donate(Nodep)		((Donate *)(Nodep->data))
#define data_s_type(Nodep)		((S_Type *)(Nodep->data))
#define data_s_name(Nodep)		((S_Name *)(Nodep->data))
#define data_s_month(Nodep)		((S_Time *)(Nodep->data))

/* common head (implement in node_operation.c)*/
extern Node *head_classify;
/* sequence head */
extern Node *head_cri[MAX_SEQUENCE_TYPE];
/* alias */
extern Node *head_classify_name;

extern Node *head_event_serial;
extern Node *head_event_name;
extern Node *head_event_time;
extern Node *head_event_level;
extern Node *head_event_loc;
extern Node *head_event_deads;
extern Node *head_event_house;
extern Node *head_event_money;

extern Node *head_donate_name;
extern Node *head_donate_type;
extern Node *head_donate_amount;
extern Node *head_donate_serial;
extern Node *head_donate_time;
/* searching result */
extern unsigned long MAX_RESULT_NUM;
extern Node	**inquire_result;
/* statistic result */
extern Node *head_statistic_type;
extern Node *head_statistic_name;

/* three types of data structure */
typedef struct{
    char 	name[20];
    char 	type;
}Classify;

#define name_classify(Nodep)	(data_classify(Nodep)->name)
#define type_classify(Nodep)	(data_classify(Nodep)->type)

typedef struct{
    char 	serial[12];
    char 	time[15];
    char 	name[20];			//disaster name
    char 	level;
    char 	location[20];
    int  	deads;
    int		house_destroyed;
    float   pencuniary_loss;	//metric: 100,000,000 CNY
}Event;

#define serial_event(Nodep)				(data_event(Nodep)->serial)
#define time_event(Nodep)				(data_event(Nodep)->time)
#define name_event(Nodep)				(data_event(Nodep)->name)
#define level_event(Nodep)				(data_event(Nodep)->level)
#define location_event(Nodep)			(data_event(Nodep)->location)
#define deads_event(Nodep)				(data_event(Nodep)->deads)
#define house_destroyed_event(Nodep)	(data_event(Nodep)->house_destroyed)
#define pencuniary_loss_event(Nodep)	(data_event(Nodep)->pencuniary_loss)

typedef struct{
    char	serial[12];
    char	time[10];
    char 	type;				//'1'for personal and '2' for enterprise
    char 	name[20];			//donor's name
    float 	amount;				//metric: 10,000 CNY
    char 	classify_name[20];
}Donate;

#define serial_donate(Nodep)	(data_donate(Nodep)->serial)
#define time_donate(Nodep)		(data_donate(Nodep)->time)
#define type_donate(Nodep)		(data_donate(Nodep)->type)
#define name_donate(Nodep)		(data_donate(Nodep)->name)
#define amount_donate(Nodep)	(data_donate(Nodep)->amount)
#define classify_name_donate(Nodep)	(data_donate(Nodep)->classify_name)

/* the structures below is for statistics */

typedef struct{
    char type;
    int times;		/* happening times */
	int deads;
	int house_destroyed;
	float pecuniary_loss;
}S_Type;	/* statistic which main-key is type*/

#define type_s_type(Nodep)					(data_s_type(Nodep)->type)
#define times_s_type(Nodep)					(data_s_type(Nodep)->times)
#define deads_s_type(Nodep)					(data_s_type(Nodep)->deads)
#define house_destroyed_s_type(Nodep)		(data_s_type(Nodep)->house_destroyed)
#define pecuniary_loss_s_type(Nodep)		(data_s_type(Nodep)->pecuniary_loss)

typedef struct{
	char name[20];
	char time[10];		/* this is only used in second class */
	int times;
	int deads;
	int house_destroyed;
	float pecuniary_loss;
}S_Name;	/* statistic which main-key is name(first-class) and time(second class) */

#define name_s_name(Nodep)					(data_s_name(Nodep)->name)
#define time_s_name(Nodep)					(data_s_name(Nodep)->time)
#define times_s_name(Nodep)					(data_s_name(Nodep)->times)
#define deads_s_name(Nodep)					(data_s_name(Nodep)->deads)
#define house_destroyed_s_name(Nodep)		(data_s_name(Nodep)->house_destroyed)
#define pecuniary_loss_s_name(Nodep)		(data_s_name(Nodep)->pecuniary_loss)


#endif // DATA_STRUCTURE_H_INCLUDE

