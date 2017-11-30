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

#include "display.h"
#include "treeview_search_result_add_data.h"
#include "treeview_all_adddata.h"
#include "data_structure.h"
#include <gtk.h>

void treeview_search_result_add_all_data(void){
	/* cleat the list and get the iter */
	gtk_list_store_clear(liststore_search_result);
	GtkTreeIter	iter;
	unsigned long counter	=	0;
	char detail_buff[1000];
	if(inquire_result[0]!=NULL){
		switch(inquire_result[counter]->node_type){
			/* insert the classify search result */
			case node_type_classify:
				for(;inquire_result[counter]!=NULL;counter++){
					sprintf(detail_buff,
						"type:%-21s name:%-21s",
						string_type_classify(type_classify(inquire_result[counter])),
						name_classify(inquire_result[counter]));
					gtk_list_store_append(GTK_LIST_STORE(liststore_search_result),&iter);
					gtk_list_store_set(GTK_LIST_STORE(liststore_search_result),&iter,
						TREE_ALL_NODE_TYPE,			"disaster",
						TREE_ALL_CLASSIFY_TYPE,		string_type_classify(type_classify(inquire_result[counter])),
						TREE_ALL_CLASSIFY_TYPE_NUM,	type_classify(inquire_result[counter]),
						TREE_ALL_CLASSIFY_NAME,		name_classify(inquire_result[counter]),
						TREE_ALL_EVENT_SERIAL,		"",
						TREE_ALL_DONATE_NAME,		"",
						TREE_ALL_DETAIL_BUFF,		detail_buff,
						TREE_ALL_NODE_ADDRESS,		inquire_result[counter],-1);
				}
				break;
			/* insert the event search result */
			case node_type_event:
				for(;inquire_result[counter]!=NULL;counter++){
					sprintf(detail_buff,
						"serial:%-13s time:%-16s level:%c location:%-21s "
						"deads:%-6d house destroyed:%-6d pecuniary loss:%.2f",
						serial_event(inquire_result[counter]),
						time_event(inquire_result[counter]),
						level_event(inquire_result[counter]),
						location_event(inquire_result[counter]),
						deads_event(inquire_result[counter]),
						house_destroyed_event(inquire_result[counter]),
						pencuniary_loss_event(inquire_result[counter]));
					gtk_list_store_append(GTK_LIST_STORE(liststore_search_result),&iter);
					gtk_list_store_set(GTK_LIST_STORE(liststore_search_result),&iter,
						TREE_ALL_NODE_TYPE,			"event",
						TREE_ALL_CLASSIFY_TYPE,		"",
						TREE_ALL_CLASSIFY_TYPE_NUM,	0,
						TREE_ALL_CLASSIFY_NAME,		"",
						TREE_ALL_EVENT_SERIAL,		serial_event(inquire_result[counter]),
						TREE_ALL_DONATE_NAME,		"",
						TREE_ALL_DETAIL_BUFF,		detail_buff,
						TREE_ALL_NODE_ADDRESS,		inquire_result[counter],-1);
				}
				break;
			/* insert the donate search result */
			case node_type_donate:
				for(;inquire_result[counter]!=NULL;counter++){
					sprintf(detail_buff,
						"time:%-16s donate type:%20s donor's name:%-30s donate amount:%.2f",
						time_donate(inquire_result[counter]),
						string_type_donate(type_donate(inquire_result[counter])),
						name_donate(inquire_result[counter]),
						amount_donate(inquire_result[counter]));
					gtk_list_store_append(GTK_LIST_STORE(liststore_search_result),&iter);
					gtk_list_store_set(GTK_LIST_STORE(liststore_search_result),&iter,
						TREE_ALL_NODE_TYPE,			"donate",
						TREE_ALL_CLASSIFY_TYPE,		"",
						TREE_ALL_CLASSIFY_TYPE_NUM,	0,
						TREE_ALL_CLASSIFY_NAME,		"",
						TREE_ALL_EVENT_SERIAL,		"",
						TREE_ALL_DONATE_NAME,		name_donate(inquire_result[counter]),
						TREE_ALL_DETAIL_BUFF,		detail_buff,
						TREE_ALL_NODE_ADDRESS,			inquire_result[counter],-1);
				}
				break;
		}	/* switch */
	}		/* if */
}
