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
#include "main_buttons.h"
#include "sub_button_statistics.h"
#include "treeview_all_adddata.h"
#include "chain_operation.h"
#include "node_operation.h"
#include "data_structure.h"
#include <gtk.h>
#include <string.h>

G_MODULE_EXPORT void on_button_statistic_event_clicked(void){
	GtkBuilder	*builder_statistic	=	gtk_builder_new();
	gtk_builder_add_from_file(builder_statistic,"statistic.ui",NULL);
	gtk_builder_connect_signals(builder_statistic,NULL);

	GtkDialog		*dialog_statistic		=	GTK_DIALOG(gtk_builder_get_object(builder_statistic,"dialog_statistic_event"));
	GtkComboBox		*combobox_type			=	GTK_COMBO_BOX(gtk_builder_get_object(builder_statistic,"comboboxtext_statistic_type"));
	GtkEntry		*entry_name				=	GTK_ENTRY(gtk_builder_get_object(builder_statistic,"entry_statistic_name"));
	GtkSpinButton	*spinbutton_level_start	=	GTK_SPIN_BUTTON(gtk_builder_get_object(builder_statistic,"spinbutton_statistic_level_start"));
	GtkSpinButton	*spinbutton_level_end	=	GTK_SPIN_BUTTON(gtk_builder_get_object(builder_statistic,"spinbutton_statistic_level_end"));
	GtkToggleButton	*checkbutton_type		=	GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder_statistic,"checkbutton1"));
	GtkToggleButton	*checkbutton_name		=	GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder_statistic,"checkbutton2"));
	GtkToggleButton	*checkbutton_level		=	GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder_statistic,"checkbutton3"));
	/* get the data */
	gtk_window_set_transient_for(GTK_WINDOW(dialog_statistic),GTK_WINDOW(window_main));
	if(gtk_dialog_run(dialog_statistic)==1){
        /* do statistics */
        chain_inquire_statistic_event(
			gtk_toggle_button_get_active(checkbutton_type)	? -1 :gtk_combo_box_get_active(combobox_type)+'0',
			gtk_toggle_button_get_active(checkbutton_level)	? -1 :gtk_spin_button_get_value_as_int(spinbutton_level_start),
			gtk_toggle_button_get_active(checkbutton_level) ? -1 :gtk_spin_button_get_value_as_int(spinbutton_level_end),
			gtk_toggle_button_get_active(checkbutton_name)	?NULL:gtk_entry_get_text(entry_name));
		/* show the result */
        GtkWidget		*window_result	=	GTK_WIDGET(gtk_builder_get_object(builder_statistic,"window_statistic_result"));
        GtkListStore	*liststore_type	=	GTK_LIST_STORE(gtk_builder_get_object(builder_statistic,"liststore_type"));
        GtkTreeStore	*treestore_event=	GTK_TREE_STORE(gtk_builder_get_object(builder_statistic,"treestore_event"));
		GtkTreeIter		iter_type,	iter_name,	iter_time;
		Node			*temp1;		/* first class*/
		Node			*temp2;		/* second class*/
		/* add data to list store */
		for(temp1=head_statistic_type->next; temp1!=NULL; temp1=temp1->next){
			gtk_list_store_append(liststore_type,&iter_type);
			gtk_list_store_set(liststore_type,&iter_type,
				0,string_type_classify(type_s_type(temp1)),
				1,times_s_type(temp1),
				2,deads_s_type(temp1),
				3,house_destroyed_s_type(temp1),
				4,pecuniary_loss_s_type(temp1),-1);
		}
		for(temp1=head_statistic_name->next; temp1!=NULL; temp1=temp1->next){
			gtk_tree_store_append(treestore_event,&iter_name,NULL);
			gtk_tree_store_set(treestore_event,&iter_name,
				0,name_s_name(temp1),
				1,times_s_name(temp1),
				2,deads_s_name(temp1),
				3,house_destroyed_s_name(temp1),
				4,pecuniary_loss_s_name(temp1),-1);
			for(temp2=temp1->child; temp2!=NULL; temp2=temp2->next){
				gtk_tree_store_append(treestore_event,&iter_time,&iter_name);
				gtk_tree_store_set(treestore_event,&iter_time,
					0,time_s_name(temp2),
					1,times_s_name(temp2),
					2,deads_s_name(temp2),
					3,house_destroyed_s_name(temp2),
					4,pecuniary_loss_s_name(temp2),-1);
			}
		}
		gtk_widget_destroy(GTK_WIDGET(dialog_statistic));
		gtk_window_set_transient_for(GTK_WINDOW(window_result),GTK_WINDOW(window_main));
		gtk_widget_show(window_result);
	}
	g_object_unref(G_OBJECT(builder_statistic));
}
G_MODULE_EXPORT void on_button_statistic_donate_clicked(void){
	GtkBuilder	*builder_statistic	=	gtk_builder_new();
	gtk_builder_add_from_file(builder_statistic,"statistic.ui",NULL);
	gtk_builder_connect_signals(builder_statistic,NULL);

	GtkEntry	*entry_time_start	=	GTK_ENTRY(gtk_builder_get_object(builder_statistic,"entry_time_start"));
	GtkEntry	*entry_time_end		=	GTK_ENTRY(gtk_builder_get_object(builder_statistic,"entry_time_end"));
	GtkDialog	*dialog_donate		=	GTK_DIALOG(gtk_builder_get_object(builder_statistic,"dialog_statistic_donate"));
	gtk_window_set_transient_for(GTK_WINDOW(dialog_donate),GTK_WINDOW(window_main));
	if(gtk_dialog_run(dialog_donate)==1){
		float	donate_enterprise[8],donate_personal[8];
		memset(donate_enterprise,0,8*sizeof(float));
		memset(donate_personal,0,8*sizeof(float));
		/* find the according ones */
		char time_start[10],time_end[10];
		strcpy(time_start,gtk_entry_get_text(entry_time_start));
		strcpy(time_end,gtk_entry_get_text(entry_time_end));
		Node *finder	=	(head_donate_time->sequence_next)[cri_donate_time];
		GtkEntry	*entry_temp;
		char buffer[20];
		for(; finder!=NULL; finder=(finder->sequence_next)[cri_donate_time]){
			if(	strcmp(time_start,time_donate(finder))	<=0
			&&	strcmp(time_donate(finder),time_end)	<=0){
				if(type_donate(finder)=='1')
					donate_personal[type_classify(finder->parent->parent)-'0']		+=	amount_donate(finder);
				else
					donate_enterprise[type_classify(finder->parent->parent)-'0']	+=	amount_donate(finder);
			}
		}
		gtk_widget_destroy(GTK_WIDGET(dialog_donate));
		/* fill the entries */
		for(int i=0;i<8;i++){
			sprintf(buffer,"entry%d",i);
			entry_temp	=	GTK_ENTRY(gtk_builder_get_object(builder_statistic,buffer));
			sprintf(buffer,"%.2f",donate_enterprise[i]);
			gtk_entry_set_text(entry_temp,buffer);
		}
		for(int i=0;i<8;i++){
			sprintf(buffer,"entry%d",i+8);
			entry_temp	=	GTK_ENTRY(gtk_builder_get_object(builder_statistic,buffer));
			sprintf(buffer,"%.2f",donate_personal[i]);
			gtk_entry_set_text(entry_temp,buffer);
		}
		GtkWidget *window_result	=	GTK_WIDGET(gtk_builder_get_object(builder_statistic,"window_statistic_donate_result"));
		gtk_window_set_transient_for(GTK_WINDOW(window_result),GTK_WINDOW(window_main));
		gtk_widget_show(window_result);
	}
	g_object_unref(G_OBJECT(builder_statistic));
}
/* output the sorted list */
G_MODULE_EXPORT void on_button_statistic_rank_clicked(void){
	GtkBuilder	*builder_statistic	=	gtk_builder_new();
	gtk_builder_add_from_file(builder_statistic,"statistic.ui",NULL);
	gtk_builder_connect_signals(builder_statistic,NULL);

    GtkDialog	*dialog_selection	=	GTK_DIALOG(gtk_builder_get_object(builder_statistic,"dialog_statistic_rank_select"));
    GtkComboBox	*combobox_selection	=	GTK_COMBO_BOX(gtk_builder_get_object(builder_statistic,"comboboxtext_selection"));
	gtk_window_set_transient_for(GTK_WINDOW(dialog_selection),GTK_WINDOW(window_main));

	if(gtk_dialog_run(dialog_selection)==1){
		criteria_t	criteria		=	gtk_combo_box_get_active(combobox_selection);
		Node		*finder			=	(head_cri[criteria]->sequence_next)[criteria];
		GtkWidget	*window_result;
		GtkListStore	*liststore;
		GtkTreeIter		iter_temp;
		if(gtk_combo_box_get_active(combobox_selection) < 0x01){
			window_result	=	GTK_WIDGET(gtk_builder_get_object(builder_statistic,"window1"));
			liststore	=	GTK_LIST_STORE(gtk_builder_get_object(builder_statistic,"liststore1"));
			for(; finder!=NULL; finder=(finder->sequence_next)[criteria]){
				gtk_list_store_append(liststore,&iter_temp);
				gtk_list_store_set(liststore,&iter_temp,
					0,string_type_classify(type_classify(finder)),
					1,name_classify(finder),-1);
			}

		}
		else if(gtk_combo_box_get_active(combobox_selection) < 0x09){
			window_result	=	GTK_WIDGET(gtk_builder_get_object(builder_statistic,"window2"));
			liststore	=	GTK_LIST_STORE(gtk_builder_get_object(builder_statistic,"liststore2"));
			for(; finder!=NULL; finder=(finder->sequence_next)[criteria]){
				gtk_list_store_append(liststore,&iter_temp);
				gtk_list_store_set(liststore,&iter_temp,
					0,name_event(finder),
					1,serial_event(finder),
					2,time_event(finder),
					3,level_event(finder)-'0',
					4,location_event(finder),
					5,deads_event(finder),
					6,house_destroyed_event(finder),
					7,pencuniary_loss_event(finder),-1);
			}
		}
		else{
			window_result	=	GTK_WIDGET(gtk_builder_get_object(builder_statistic,"window3"));
			liststore	=	GTK_LIST_STORE(gtk_builder_get_object(builder_statistic,"liststore3"));
			for(; finder!=NULL; finder=(finder->sequence_next)[criteria]){
				gtk_list_store_append(liststore,&iter_temp);
				gtk_list_store_set(liststore,&iter_temp,
					0,serial_donate(finder),
					1,time_donate(finder),
					2,string_type_donate(type_donate(finder)),
					3,name_donate(finder),
					4,amount_donate(finder),-1);
			}
		}
		gtk_widget_destroy(GTK_WIDGET(dialog_selection));
		gtk_window_set_transient_for(GTK_WINDOW(window_result),GTK_WINDOW(window_main));
		gtk_widget_show(window_result);
	}
}
