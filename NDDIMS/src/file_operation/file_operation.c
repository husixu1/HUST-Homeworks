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

#include "file_operation.h"
#include "node_operation.h"
#include "data_structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* global variable, shows the current opened file(in tree view), empty for no file open */
char file_current_name[MAX_FILENAME_LEN]	=	"";
error_t file_save(const char *file_name){
    Node *temp1;	//node_type_classify
    Node *temp2;	//node_type_event
    Node *temp3;	//node_type_donate
	/* save into file*/
	FILE *f_temp	=	fopen(file_name,"wb");
    for(temp1=head_classify->next;temp1!=NULL;temp1=temp1->next){
		fwrite(temp1,sizeof(Node),1,f_temp);
		fwrite(temp1->data,sizeof(Classify),1,f_temp);
		for(temp2=temp1->child;temp2!=NULL;temp2=temp2->next){
			fwrite(temp2,sizeof(Node),1,f_temp);
			fwrite(temp2->data,sizeof(Event),1,f_temp);
			for(temp3=temp2->child;temp3!=NULL;temp3=temp3->next){
				fwrite(temp3,sizeof(Node),1,f_temp);
				fwrite(temp3->data,sizeof(Donate),1,f_temp);
			}
		}
	}
	fclose(f_temp);
	return no_error;
}
error_t file_load(const char *file_name){
	FILE *file = fopen(file_name,"rb");
	if(head_classify->next!=NULL)
		return error_initialized;
	Node *temp;
	for(;;){
		temp = (Node *)malloc(sizeof(Node));
		if(temp==NULL)
			return error_malloc;
		if(fread(temp,sizeof(Node),1,file)==0)
			break;
		__node_clear_pointer(temp);
		switch(temp->node_type){
			case node_type_classify:
				temp->data	=	(void *)malloc(sizeof(Classify));
				fread(temp->data,sizeof(Classify),1,file);
				break;
			case node_type_event:
				temp->data	=	(void *)malloc(sizeof(Event));
				fread(temp->data,sizeof(Event),1,file);
				break;
			case node_type_donate:
				temp->data	=	(void *)malloc(sizeof(Donate));
				fread(temp->data,sizeof(Donate),1,file);
				break;
			default:
				return error_parameter;
		}
		node_insert(temp);
	}
	strcpy(file_current_name,file_name);
	fclose(file);
	return no_error;
}
