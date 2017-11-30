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
#include "sub_button_searching.h"
#include "main_buttons.h"
#include "treeview_search_result.h"
#include "treeview_search_result_add_data.h"
#include "chain_operation.h"
#include "node_operation.h"
#include "data_structure.h"
#include <gtk.h>
#include <string.h>

/* the disaster searching dialog */
G_MODULE_EXPORT void on_button_searching_disaster_clicked(void){
	GtkBuilder	*builder_searching	=	gtk_builder_new();
    GtkWidget	*dialog_searching;
    GtkWidget	*combobox_text_type;
    GtkWidget	*entry_name;

    gtk_builder_add_from_file(builder_searching,"searching.ui",NULL);
    gtk_builder_connect_signals(builder_searching,NULL);
	dialog_searching	=	GTK_WIDGET(gtk_builder_get_object(builder_searching,"dialog_searching_classfy"));
	combobox_text_type	=	GTK_WIDGET(gtk_builder_get_object(builder_searching,"combobox_text_type"));
	entry_name			=	GTK_WIDGET(gtk_builder_get_object(builder_searching,"entry_classify_name"));

	gtk_window_set_transient_for(GTK_WINDOW(dialog_searching),GTK_WINDOW(window_main));

	if(gtk_dialog_run(GTK_DIALOG(dialog_searching))==1){
		char type	=	gtk_combo_box_get_active(GTK_COMBO_BOX(combobox_text_type))+'0';
		char name[20];
		strcpy(name,gtk_entry_get_text(GTK_ENTRY(entry_name)));
		/* find the nodes */
		chain_inquire_classify(type,name);
		/* add the data to the list store */
		treeview_search_result_add_all_data();
		/* display the data */
		search_result_show_window();
		gtk_widget_destroy(dialog_searching);
	}
	__restore_menu_toggle_button();
    g_object_unref(G_OBJECT(builder_searching));
}

G_MODULE_EXPORT void on_button_searching_event_clicked(void){
	GtkBuilder		*builder_searching	=	gtk_builder_new();
    GtkWidget		*dialog_searching;
    GtkEntry		*entry_name;
	GtkEntry		*entry_serial_start,		*entry_serial_end;
	GtkEntry		*entry_time_start,			*entry_time_end;
	GtkSpinButton	*spinbutton_level_start,	*spinbutton_level_end;
	GtkSpinButton	*spinbutton_death_start,	*spinbutton_death_end;
	GtkSpinButton	*spinbutton_house_start,	*spinbutton_house_end;
	GtkSpinButton	*spinbutton_money_start,	*spinbutton_money_end;
	GtkCheckButton	*checkbutton_name,	*checkbutton_serial,	*checkbutton_time,
					*checkbutton_level,	*checkbutton_death,		*checkbutton_house,
					*checkbutton_money;
	/* retrieve the widgets */
    gtk_builder_add_from_file(builder_searching,"searching.ui",NULL);
    gtk_builder_connect_signals(builder_searching,NULL);
    dialog_searching		=	GTK_WIDGET(gtk_builder_get_object(builder_searching,"dialog_searching_event"));
	entry_name				=	GTK_ENTRY(gtk_builder_get_object(builder_searching,"entry_event_disaster_name_search"));
	entry_serial_start		=	GTK_ENTRY(gtk_builder_get_object(builder_searching,"entry_event_serial_search_start"));
	entry_serial_end		=	GTK_ENTRY(gtk_builder_get_object(builder_searching,"entry_event_serial_search_end"));
	entry_time_start		=	GTK_ENTRY(gtk_builder_get_object(builder_searching,"entry_event_time_search_start"));
	entry_time_end			=	GTK_ENTRY(gtk_builder_get_object(builder_searching,"entry_event_time_search_end"));
	spinbutton_level_start	=	GTK_SPIN_BUTTON(gtk_builder_get_object(builder_searching,"spinbutton_event_level_search_start"));
	spinbutton_level_end	=	GTK_SPIN_BUTTON(gtk_builder_get_object(builder_searching,"spinbutton_event_level_search_end"));
	spinbutton_death_start	=	GTK_SPIN_BUTTON(gtk_builder_get_object(builder_searching,"spinbutton_event_deads_search_start"));
	spinbutton_death_end	=	GTK_SPIN_BUTTON(gtk_builder_get_object(builder_searching,"spinbutton_event_deads_search_end"));
	spinbutton_house_start	=	GTK_SPIN_BUTTON(gtk_builder_get_object(builder_searching,"spinbutton_event_house_search_start"));
	spinbutton_house_end	=	GTK_SPIN_BUTTON(gtk_builder_get_object(builder_searching,"spinbutton_event_house_search_end"));
	spinbutton_money_start	=	GTK_SPIN_BUTTON(gtk_builder_get_object(builder_searching,"spinbutton_event_money_search_start"));
	spinbutton_money_end	=	GTK_SPIN_BUTTON(gtk_builder_get_object(builder_searching,"spinbutton_event_money_search_end"));
	checkbutton_name		=	GTK_CHECK_BUTTON(gtk_builder_get_object(builder_searching,"checkbutton1"));
	checkbutton_serial		=	GTK_CHECK_BUTTON(gtk_builder_get_object(builder_searching,"checkbutton2"));
	checkbutton_time		=	GTK_CHECK_BUTTON(gtk_builder_get_object(builder_searching,"checkbutton3"));
	checkbutton_level		=	GTK_CHECK_BUTTON(gtk_builder_get_object(builder_searching,"checkbutton4"));
	checkbutton_death		=	GTK_CHECK_BUTTON(gtk_builder_get_object(builder_searching,"checkbutton5"));
	checkbutton_house		=	GTK_CHECK_BUTTON(gtk_builder_get_object(builder_searching,"checkbutton6"));
	checkbutton_money		=	GTK_CHECK_BUTTON(gtk_builder_get_object(builder_searching,"checkbutton7"));

	gtk_window_set_transient_for(GTK_WINDOW(dialog_searching),GTK_WINDOW(window_main));
	if(gtk_dialog_run(GTK_DIALOG(dialog_searching))==1){
		/* get the data */
		chain_inquire_event(
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_serial))?NULL:gtk_entry_get_text(entry_serial_start),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_serial))?NULL:gtk_entry_get_text(entry_serial_end),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_name  ))?NULL:gtk_entry_get_text(entry_name),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_time	 ))?NULL:gtk_entry_get_text(entry_time_start),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_time  ))?NULL:gtk_entry_get_text(entry_time_end),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_level ))? -1 :gtk_spin_button_get_value_as_int(spinbutton_level_start)+'0',
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_level ))? -1 :gtk_spin_button_get_value_as_int(spinbutton_level_end)+'0',
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_death ))? -1 :gtk_spin_button_get_value_as_int(spinbutton_death_start),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_death ))? -1 :gtk_spin_button_get_value_as_int(spinbutton_death_end),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_house ))? -1 :gtk_spin_button_get_value_as_int(spinbutton_house_start),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_house ))? -1 :gtk_spin_button_get_value_as_int(spinbutton_house_end),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_money ))? -1 :gtk_spin_button_get_value(spinbutton_money_start),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_money ))? -1 :gtk_spin_button_get_value(spinbutton_money_end));
		treeview_search_result_add_all_data();
		/* display the data */
		search_result_show_window();
		gtk_widget_destroy(dialog_searching);
	}
	__restore_menu_toggle_button();
    g_object_unref(G_OBJECT(builder_searching));
}

G_MODULE_EXPORT void on_button_searching_donate_clicked(void){
	GtkBuilder		*builder_searching	=	gtk_builder_new();
    GtkWidget		*dialog_searching;
    GtkEntry		*entry_classify_name;
    GtkEntry		*entry_name;
	GtkEntry		*entry_serial_start,		*entry_serial_end;
	GtkEntry		*entry_time_start,			*entry_time_end;
	GtkComboBox		*combobox_type;
	GtkSpinButton	*spinbutton_amount_start,	*spinbutton_amount_end;
	GtkCheckButton	*checkbutton_name,			*checkbutton_serial,	*checkbutton_time,
					*checkbutton_classify_name,	*checkbutton_amount,	*checkbutton_type;
	/* retrieve the widgets */
    gtk_builder_add_from_file(builder_searching,"searching.ui",NULL);
    gtk_builder_connect_signals(builder_searching,NULL);
    dialog_searching		=	GTK_WIDGET(gtk_builder_get_object(builder_searching,"dialog_searching_donate"));
    entry_classify_name		=	GTK_ENTRY(gtk_builder_get_object(builder_searching,"entry_donate_disaster_name_search"));
	entry_name				=	GTK_ENTRY(gtk_builder_get_object(builder_searching,"entry_donate_name_search"));
	entry_serial_start		=	GTK_ENTRY(gtk_builder_get_object(builder_searching,"entry_donate_event_serial_search_start"));
	entry_serial_end		=	GTK_ENTRY(gtk_builder_get_object(builder_searching,"entry_donate_event_serial_search_end"));
	entry_time_start		=	GTK_ENTRY(gtk_builder_get_object(builder_searching,"entry_donate_time_search_start"));
	entry_time_end			=	GTK_ENTRY(gtk_builder_get_object(builder_searching,"entry_donate_time_search_end"));
	spinbutton_amount_start	=	GTK_SPIN_BUTTON(gtk_builder_get_object(builder_searching,"spinbutton_donate_amount_search_start"));
	spinbutton_amount_end	=	GTK_SPIN_BUTTON(gtk_builder_get_object(builder_searching,"spinbutton_donate_amount_search_end"));
	combobox_type			=	GTK_COMBO_BOX(gtk_builder_get_object(builder_searching,"comboboxtext_donate_type_search"));
	checkbutton_classify_name	=	GTK_CHECK_BUTTON(gtk_builder_get_object(builder_searching,"checkbutton8"));
	checkbutton_serial			=	GTK_CHECK_BUTTON(gtk_builder_get_object(builder_searching,"checkbutton9"));
	checkbutton_name			=	GTK_CHECK_BUTTON(gtk_builder_get_object(builder_searching,"checkbutton10"));
	checkbutton_type			=	GTK_CHECK_BUTTON(gtk_builder_get_object(builder_searching,"checkbutton11"));
	checkbutton_time			=	GTK_CHECK_BUTTON(gtk_builder_get_object(builder_searching,"checkbutton12"));
	checkbutton_amount			=	GTK_CHECK_BUTTON(gtk_builder_get_object(builder_searching,"checkbutton13"));

	gtk_window_set_transient_for(GTK_WINDOW(dialog_searching),GTK_WINDOW(window_main));
	if(gtk_dialog_run(GTK_DIALOG(dialog_searching))==1){
		/* get the data */
		chain_inquire_donate(
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_name			))?NULL:gtk_entry_get_text(entry_name),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_serial		))?NULL:gtk_entry_get_text(entry_serial_start),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_serial		))?NULL:gtk_entry_get_text(entry_serial_end),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_time			))?NULL:gtk_entry_get_text(entry_time_start),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_time			))?NULL:gtk_entry_get_text(entry_time_end),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_amount		))? -1 :gtk_spin_button_get_value(spinbutton_amount_start),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_amount		))? -1 :gtk_spin_button_get_value(spinbutton_amount_end),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_classify_name))?NULL:gtk_entry_get_text(entry_classify_name),
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_type			))? -1 :gtk_combo_box_get_active(combobox_type)+'0');
		treeview_search_result_add_all_data();
		/* display the data */
		search_result_show_window();
		gtk_widget_destroy(dialog_searching);
	}
	__restore_menu_toggle_button();
    g_object_unref(G_OBJECT(builder_searching));
}
