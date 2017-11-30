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
#include "sub_button_data_management.h"
#include "treeview_all.h"
#include "file_operation.h"
#include "node_operation.h"
#include "data_structure.h"
#include <gtk.h>
#include <string.h>

G_MODULE_EXPORT void on_data_management_insert_clicked(void){
	GtkWidget	*dialog_selection;
	GtkBuilder	*builder_builder	=	gtk_builder_new();
	GtkBuilder	*builder_dialogs	=	gtk_builder_new();
	Node *target;
	gtk_builder_add_from_file(builder_builder,"builder.ui",NULL);
	gtk_builder_add_from_file(builder_dialogs,"dialogs.ui",NULL);
	gtk_builder_connect_signals(builder_builder,NULL);
	gtk_builder_connect_signals(builder_dialogs,NULL);
	/* if no file currently open */
	if(file_current_name[0]=='\0'){
		dialog_selection	=	GTK_WIDGET(gtk_builder_get_object(builder_dialogs,"dialog_no_file_open"));
		gtk_window_set_transient_for(GTK_WINDOW(dialog_selection),GTK_WINDOW(window_main));
		gtk_dialog_run(GTK_DIALOG(dialog_selection));
		g_object_unref(G_OBJECT(builder_builder));
		g_object_unref(G_OBJECT(builder_dialogs));
		return;
	}
	dialog_selection	=	GTK_WIDGET(gtk_builder_get_object(builder_dialogs,"dialog_kind_selection"));
	gtk_window_set_transient_for(GTK_WINDOW(dialog_selection),GTK_WINDOW(window_main));
	switch(gtk_dialog_run(GTK_DIALOG(dialog_selection))){
		/* classify */
		case 1:{
			gtk_widget_destroy(GTK_WIDGET(dialog_selection));
			GtkWidget	*dialog_data	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"classify_modify_dialog"));
			gtk_window_set_transient_for(GTK_WINDOW(dialog_data),GTK_WINDOW(window_main));
			/* show the classify node modify dialog */
			if(gtk_dialog_run(GTK_DIALOG(dialog_data))==1){
				GtkWidget	*entry_classify_name;
				GtkWidget	*combobox_classify_type;
				/* get entries */
				entry_classify_name		=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"entry_classify_name"));
				combobox_classify_type	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"combobox_classify_type"));
				/* get data and insert the node */
				node_init(&target,node_type_classify);
				strcpy(name_classify(target),gtk_entry_get_text(GTK_ENTRY(entry_classify_name)));
				type_classify(target) = '0'+gtk_combo_box_get_active(GTK_COMBO_BOX(combobox_classify_type));
			}
			gtk_widget_destroy(dialog_data);
		}
			goto default_goto;
			break;
		/* event */
		case 2:{
			gtk_widget_destroy(GTK_WIDGET(dialog_selection));
			GtkWidget	*dialog_data	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"event_add_dialog"));
			gtk_window_set_transient_for(GTK_WINDOW(dialog_data),GTK_WINDOW(window_main));
			/* show the event modify dialog */
			if(gtk_dialog_run(GTK_DIALOG(dialog_data))==1){
				GtkWidget	*entry_event_name;
				GtkWidget	*entry_event_serial;
				GtkWidget	*entry_event_time;
				GtkWidget	*spinbutton_event_level;
				GtkWidget	*entry_event_location;
				GtkWidget	*spinbutton_event_deads;
				GtkWidget	*spinbutton_event_house;
				GtkWidget	*spinbutton_event_money;
				/* get entries */
				entry_event_name		=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"entry_event_disaster_name_add"));
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
		/* donate */
		case 3:{
			gtk_widget_destroy(GTK_WIDGET(dialog_selection));
			GtkWidget	*dialog_data	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"donate_add_dialog"));
			gtk_window_set_transient_for(GTK_WINDOW(dialog_data),GTK_WINDOW(window_main));
			/* run the donate modify dialog */
			if(gtk_dialog_run(GTK_DIALOG(dialog_data))==1){
				GtkWidget	*entry_donate_disaster_name;
				GtkWidget	*entry_donate_name;
				GtkWidget	*entry_donate_serial;
				GtkWidget	*comboboxtext_donate_type;
				GtkWidget	*entry_donate_time;
				GtkWidget	*spinbutton_donate_amount;
				/* get the entries */
				entry_donate_disaster_name	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"entry_donate_disaster_name_add"));
				entry_donate_serial			=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"entry_donate_event_serial_add"));
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
		default:
			gtk_widget_destroy(GTK_WIDGET(dialog_selection));
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
				case error_duplicate:{
					GtkDialog	*error	=	GTK_DIALOG(gtk_builder_get_object(builder_dialogs,"dialog_error_parameter"));
					gtk_window_set_transient_for(GTK_WINDOW(error),GTK_WINDOW(window_main));
					gtk_dialog_run(error);
				}
			}
			treeview_all_reload();
			break;
	}
	__restore_menu_toggle_button();
	g_object_unref(G_OBJECT(builder_builder));
	g_object_unref(G_OBJECT(builder_dialogs));
}

G_MODULE_EXPORT void on_data_management_delete_clicked(void){
	GtkBuilder	*builder_dialog	=	gtk_builder_new();
	gtk_builder_add_from_file(builder_dialog,"dialogs.ui",NULL);
	gtk_builder_connect_signals(builder_dialog,NULL);

	GtkWidget	*dialog		=	GTK_WIDGET(gtk_builder_get_object(builder_dialog,"dialog_data_management_warning"));
	GtkLabel	*label		=	GTK_LABEL(gtk_builder_get_object(builder_dialog,"dialog_data_management_label"));
	g_object_unref(G_OBJECT(builder_dialog));

	gtk_label_set_text(label,	"Please directly choose\n"
								"the information you want to delete\n"
								"right click it and select \"delete\"\n");
	gtk_window_set_transient_for(GTK_WINDOW(dialog),GTK_WINDOW(window_main));
	gtk_dialog_run(GTK_DIALOG(dialog));
}

G_MODULE_EXPORT void on_data_management_modify_clicked(void){
	GtkBuilder	*builder_dialog	=	gtk_builder_new();
	gtk_builder_add_from_file(builder_dialog,"dialogs.ui",NULL);
	gtk_builder_connect_signals(builder_dialog,NULL);

	GtkWidget	*dialog		=	GTK_WIDGET(gtk_builder_get_object(builder_dialog,"dialog_data_management_warning"));
	GtkLabel	*label		=	GTK_LABEL(gtk_builder_get_object(builder_dialog,"dialog_data_management_label"));
	g_object_unref(G_OBJECT(builder_dialog));

	gtk_label_set_text(label,	"Please directly choose\n"
								"the information you want to modify\n"
								"double click it and modify the data\n");
	gtk_window_set_transient_for(GTK_WINDOW(dialog),GTK_WINDOW(window_main));
	gtk_dialog_run(GTK_DIALOG(dialog));
}
