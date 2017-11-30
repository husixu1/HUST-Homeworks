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
#include "treeview_all.h"
#include "treeview_all_rightclick.h"
#include "treeview_all_adddata.h"
#include "file_operation.h"
#include "node_operation.h"
#include "data_structure.h"
#include <gtk.h>
#include <string.h>
#include <stdlib.h>

G_MODULE_EXPORT void on_menuitem_add_child_activate(void){
	GtkTreeIter			*iter_temp	=	(GtkTreeIter *)malloc(sizeof(GtkTreeIter));
    GtkTreeModel		*tree_model_temp;
	GtkTreeSelection	*selection;

	GtkBuilder	*builder_builder	=	gtk_builder_new();
	GtkBuilder	*builder_dialogs	=	gtk_builder_new();
	Node *target;
	gtk_builder_add_from_file(builder_builder,"builder.ui",NULL);
	gtk_builder_add_from_file(builder_dialogs,"dialogs.ui",NULL);
	gtk_builder_connect_signals(builder_builder,NULL);
	gtk_builder_connect_signals(builder_dialogs,NULL);
	/* if no file currently open */
	if(file_current_name[0]=='\0'){
		GtkWidget *dialog_selection	=	GTK_WIDGET(gtk_builder_get_object(builder_dialogs,"dialog_no_file_open"));
		gtk_window_set_transient_for(GTK_WINDOW(dialog_selection),GTK_WINDOW(window_main));
		gtk_dialog_run(GTK_DIALOG(dialog_selection));
		g_object_unref(G_OBJECT(builder_builder));
		g_object_unref(G_OBJECT(builder_dialogs));
		return;
	}
	selection		=	gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_main));
	gboolean judge	=	gtk_tree_selection_get_selected(selection,&tree_model_temp,iter_temp);
	if(judge==0){
		free(iter_temp);
		return;
	}
	gtk_tree_model_get(tree_model_temp,iter_temp,TREE_ALL_NODE_ADDRESS,&target,-1);
	switch(target->node_type){
		/* classify ,add event */
		case node_type_classify:{
			GtkWidget	*dialog_data	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"event_add_dialog"));
			gtk_window_set_transient_for(GTK_WINDOW(dialog_data),GTK_WINDOW(window_main));
			GtkEntry	*entry_event_name		=	GTK_ENTRY(gtk_builder_get_object(builder_builder,"entry_event_disaster_name_add"));
			gtk_entry_set_text(entry_event_name,name_classify(target));
			gtk_editable_set_editable(GTK_EDITABLE(entry_event_name),FALSE);
			/* show the event modify dialog */
			if(gtk_dialog_run(GTK_DIALOG(dialog_data))==1){
				GtkWidget	*entry_event_serial;
				GtkWidget	*entry_event_time;
				GtkWidget	*spinbutton_event_level;
				GtkWidget	*entry_event_location;
				GtkWidget	*spinbutton_event_deads;
				GtkWidget	*spinbutton_event_house;
				GtkWidget	*spinbutton_event_money;
				/* get entries */
				entry_event_serial		=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"entry_event_serial_add"));
				entry_event_time		=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"entry_event_time_add"));
				spinbutton_event_level	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"spinbutton_event_level_add"));
				entry_event_location	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"entry_event_location_add"));
				spinbutton_event_deads	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"spinbutton_event_deads_add"));
				spinbutton_event_house	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"spinbutton_event_house_add"));
				spinbutton_event_money	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"spinbutton_event_money_add"));
				/* get data and insert the node */
				node_init(&target,node_type_event);
				strcpy(name_event(target),gtk_entry_get_text(GTK_ENTRY(entry_event_name)));
				strcpy(serial_event(target),gtk_entry_get_text(GTK_ENTRY(entry_event_serial)));
				strcpy(time_event(target),gtk_entry_get_text(GTK_ENTRY(entry_event_time)));
				strcpy(location_event(target),gtk_entry_get_text(GTK_ENTRY(entry_event_location)));
				level_event(target)				=	gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbutton_event_level))+'0';
				deads_event(target)				=	gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbutton_event_deads));
				house_destroyed_event(target)	=	gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbutton_event_house));
				pencuniary_loss_event(target)	=	gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbutton_event_money));
			}
			gtk_widget_destroy(dialog_data);
		}
			goto default_goto;
			break;
		/* event, add donate */
		case node_type_event:{
			GtkWidget	*dialog_data	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"donate_add_dialog"));
			GtkWidget	*entry_donate_disaster_name	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"entry_donate_disaster_name_add"));
			GtkWidget	*entry_donate_serial			=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"entry_donate_event_serial_add"));
			gtk_entry_set_text(GTK_ENTRY(entry_donate_disaster_name),name_event(target));
			gtk_entry_set_text(GTK_ENTRY(entry_donate_serial),serial_event(target));
			gtk_editable_set_editable(GTK_EDITABLE(entry_donate_disaster_name),FALSE);
			gtk_editable_set_editable(GTK_EDITABLE(entry_donate_serial),FALSE);
			gtk_window_set_transient_for(GTK_WINDOW(dialog_data),GTK_WINDOW(window_main));
			/* run the donate modify dialog */
			if(gtk_dialog_run(GTK_DIALOG(dialog_data))==1){
				GtkWidget	*entry_donate_name;
				GtkWidget	*comboboxtext_donate_type;
				GtkWidget	*entry_donate_time;
				GtkWidget	*spinbutton_donate_amount;
				/* get the entries */
				entry_donate_name			=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"entry_donate_name_add"));
				comboboxtext_donate_type	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"comboboxtext_donate_type_add"));
				entry_donate_time			=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"entry_donate_time_add"));
				spinbutton_donate_amount	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"spinbutton_donate_amount_add"));
				/* insert the node */
				node_init(&target,node_type_donate);
				strcpy(classify_name_donate(target),gtk_entry_get_text(GTK_ENTRY(entry_donate_disaster_name)));
				strcpy(serial_donate(target),gtk_entry_get_text(GTK_ENTRY(entry_donate_serial)));
				strcpy(name_donate(target),gtk_entry_get_text(GTK_ENTRY(entry_donate_name)));
				strcpy(time_donate(target),gtk_entry_get_text(GTK_ENTRY(entry_donate_time)));
				amount_donate(target)	=	gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbutton_donate_amount));
				type_donate(target)		=	gtk_combo_box_get_active(GTK_COMBO_BOX(comboboxtext_donate_type))+'1';
			}
			gtk_widget_destroy(dialog_data);
		}
			goto default_goto;
			break;
		/* insert the node and handle the errors */
		default_goto:
			switch(node_insert(target)){
				case error_no_parent:{
					GtkDialog	*error	=	GTK_DIALOG(gtk_builder_get_object(builder_dialogs,"dialog_error_no_parent"));
					gtk_window_set_transient_for(GTK_WINDOW(error),GTK_WINDOW(window_main));
					gtk_dialog_run(error);
				}
					break;
				case error_parameter:{
					GtkDialog	*error	=	GTK_DIALOG(gtk_builder_get_object(builder_dialogs,"dialog_error_parameter"));
					gtk_window_set_transient_for(GTK_WINDOW(error),GTK_WINDOW(window_main));
					gtk_dialog_run(error);
				}
					break;
			}
			treeview_all_reload();
			break;
	}
	free(iter_temp);
	__restore_menu_toggle_button();
	g_object_unref(G_OBJECT(builder_builder));
	g_object_unref(G_OBJECT(builder_dialogs));
}

/* delete a node, ask if delete the child(if has) */
G_MODULE_EXPORT void on_menuitem_delete_activate(void){
	GtkTreeIter			*iter_temp	=	(GtkTreeIter *)malloc(sizeof(GtkTreeIter));
    GtkTreeModel		*tree_model_temp;
	GtkTreeSelection	*selection;

	GtkBuilder	*builder_dialogs	=	gtk_builder_new();
	Node *target;
	gtk_builder_add_from_file(builder_dialogs,"dialogs.ui",NULL);
	gtk_builder_connect_signals(builder_dialogs,NULL);
	/* get the selection row */
	selection	=	gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_main));
	gtk_tree_selection_get_selected(selection,&tree_model_temp,iter_temp);
	if(iter_temp==NULL){
		free(iter_temp);
		return;
	}
	/* get the node*/
	gtk_tree_model_get(tree_model_temp,iter_temp,TREE_ALL_NODE_ADDRESS,&target,-1);
	if(target->child!=NULL){
		GtkDialog	*dialog_child_not_empty	=	GTK_DIALOG(gtk_builder_get_object(builder_dialogs,"dialog_open_confirm"));
		GtkLabel	*label_warning			=	GTK_LABEL(gtk_builder_get_object(builder_dialogs,"label1"));
		gtk_label_set_text(label_warning,"This information has child data\n"
			" Are you really going to delete?");
		gtk_window_set_transient_for(GTK_WINDOW(dialog_child_not_empty),GTK_WINDOW(window_main));
		if(gtk_dialog_run(dialog_child_not_empty)==1){
			node_delete_recursive(target);
			gtk_tree_store_remove(GTK_TREE_STORE(tree_model_temp),iter_temp);
			gtk_widget_destroy(GTK_WIDGET(dialog_child_not_empty));
		}
	}
	else{
		node_delete(target);
		gtk_tree_store_remove(GTK_TREE_STORE(tree_model_temp),iter_temp);
	}
	free(iter_temp);
	g_object_unref(G_OBJECT(builder_dialogs));
}
