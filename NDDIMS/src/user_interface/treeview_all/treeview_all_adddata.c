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

/* this layer connect the user interface and node_operation */

#include "treeview_all_adddata.h"
#include "node_operation.h"
#include "data_structure.h"
#include <gtk/gtk.h>
#include <stdio.h>

void treeview_add_all_data(GtkTreeStore *tree){

	GtkTreeIter	iter1;
	GtkTreeIter	iter2;
	GtkTreeIter iter3;

    Node	*temp1;	/* node_type_classify	*/
    Node	*temp2;	/* node_type_event		*/
    Node	*temp3;	/* node_type_donate		*/

	char	detail_buff[MAX_DETAIL_BUF_SIZE];

    for(temp1=head_classify->next;temp1!=NULL;temp1=temp1->next){
		sprintf(detail_buff,
			"type:%-21s name:%-21s",
			string_type_classify(type_classify(temp1)),
			name_classify(temp1));
		gtk_tree_store_append(tree,&iter1,NULL);
		gtk_tree_store_set(tree,&iter1,
			TREE_ALL_NODE_TYPE,			"disaster",
			TREE_ALL_CLASSIFY_TYPE,		string_type_classify(type_classify(temp1)),
			TREE_ALL_CLASSIFY_TYPE_NUM,	type_classify(temp1),
			TREE_ALL_CLASSIFY_NAME,		name_classify(temp1),
			TREE_ALL_EVENT_SERIAL,		"",
			TREE_ALL_DONATE_NAME,		"",
			TREE_ALL_DETAIL_BUFF,		detail_buff,
			TREE_ALL_NODE_ADDRESS,			temp1,-1);
		for(temp2=temp1->child;temp2!=NULL;temp2=temp2->next){
			sprintf(detail_buff,
				"serial:%-13s time:%-16s level:%c location:%-21s "
				"deads:%-6d house destroyed:%-6d pecuniary loss:%.2f",
				serial_event(temp2),
				time_event(temp2),
				level_event(temp2),
				location_event(temp2),
				deads_event(temp2),
				house_destroyed_event(temp2),
				pencuniary_loss_event(temp2));
			gtk_tree_store_append(tree,&iter2,&iter1);
			gtk_tree_store_set(tree,&iter2,
				TREE_ALL_NODE_TYPE,			"event",
				TREE_ALL_CLASSIFY_TYPE,		"",
				TREE_ALL_CLASSIFY_TYPE_NUM,	0,
				TREE_ALL_CLASSIFY_NAME,		"",
				TREE_ALL_EVENT_SERIAL,		serial_event(temp2),
				TREE_ALL_DONATE_NAME,		"",
				TREE_ALL_DETAIL_BUFF,		detail_buff,
				TREE_ALL_NODE_ADDRESS,			temp2,-1);
			for(temp3=temp2->child;temp3!=NULL;temp3=temp3->next){
				sprintf(detail_buff,
					"time:%-16s donate type:%-15s donor's name:%-30s donate amount:%.2f",
					time_donate(temp3),
					string_type_donate(type_donate(temp3)),
					name_donate(temp3),
					amount_donate(temp3));
				gtk_tree_store_append(tree,&iter3,&iter2);
				gtk_tree_store_set(tree,&iter3,
					TREE_ALL_NODE_TYPE,			"donate",
					TREE_ALL_CLASSIFY_TYPE,		"",
					TREE_ALL_CLASSIFY_TYPE_NUM,	0,
					TREE_ALL_CLASSIFY_NAME,		"",
					TREE_ALL_EVENT_SERIAL,		"",
					TREE_ALL_DONATE_NAME,		name_donate(temp3),
					TREE_ALL_DETAIL_BUFF,		detail_buff,
					TREE_ALL_NODE_ADDRESS,			temp3,-1);
			}
		}
	}
}

