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
#include "treeview_all_modify.h"
#include "treeview_all_adddata.h"
#include "data_structure.h"
#include <gtk.h>
#include <stdlib.h>
#include <string.h>

void run_classify_modify_dialog(
	Node			*target,
	GtkTreeModel	*tree_model_temp,
	GtkTreeIter		*iter_temp){
	/* used to modify the event node in tree store*/
	GtkWidget	*data_modify_dialog;
	GtkWidget	*entry_classify_name;
	GtkWidget	*combobox_classify_type;
	GtkBuilder	*builder_temp	=	gtk_builder_new();
	/* if the main of a superior node has changed the child of is will be changed too */
	/* in case the text changed when changing the child node */
	char mainkeybuffer[30];
	char detail_buff[200];
	char classify_type;

	/* show the dialog to modify the node data */
	gtk_builder_add_from_file(builder_temp,"builder.ui",NULL);
	data_modify_dialog		=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"classify_modify_dialog"));
	entry_classify_name		=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"entry_classify_name"));
	combobox_classify_type	=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"combobox_classify_type"));
	gtk_window_set_transient_for(GTK_WINDOW(data_modify_dialog),GTK_WINDOW(window_main));
	g_object_unref(G_OBJECT(builder_temp));

	/* set the default text */
	gtk_tree_model_get(tree_model_temp,iter_temp,TREE_ALL_CLASSIFY_TYPE_NUM,&classify_type,-1);
	gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_classify_type),classify_type-'0');
	gtk_entry_set_text(GTK_ENTRY(entry_classify_name),name_classify(target));

	/* if apply the change */
	if(gtk_dialog_run(GTK_DIALOG(data_modify_dialog))==1){
		/* get the data & change the data in target node (in chain) */
		strcpy(mainkeybuffer,gtk_entry_get_text(GTK_ENTRY(entry_classify_name)));
		strcpy(name_classify(target),mainkeybuffer);
		type_classify(target) = '0'+gtk_combo_box_get_active(GTK_COMBO_BOX(combobox_classify_type));
		/*  change the child data in chain */
		Node *temp1;	/* event  changer */
		Node *temp2;	/* donate changer */
		for(temp1=target->child;temp1!=NULL;temp1=temp1->next){
			strcpy(name_event(temp1),mainkeybuffer);
			for(temp2=temp1->child;temp2!=NULL;temp2=temp2->next)
				strcpy(classify_name_donate(temp2),mainkeybuffer);
		}
		/* change the presentation in tree view */
		sprintf(detail_buff,
			"type:%-21s name:%-21s",
			string_type_classify(type_classify(target)),
			name_classify(target));
		gtk_tree_store_set(GTK_TREE_STORE(tree_model_temp),iter_temp,
			TREE_ALL_NODE_TYPE,			"disaster",
			TREE_ALL_CLASSIFY_TYPE,		string_type_classify(type_classify(target)),
			TREE_ALL_CLASSIFY_TYPE_NUM,	type_classify(target),
			TREE_ALL_CLASSIFY_NAME,		name_classify(target),
			TREE_ALL_EVENT_SERIAL,		"",
			TREE_ALL_DONATE_NAME,		"",
			TREE_ALL_DETAIL_BUFF,		detail_buff,
			TREE_ALL_NODE_ADDRESS,			target,-1);
	}
	gtk_widget_destroy(GTK_WIDGET(data_modify_dialog));
	free(iter_temp);
}

void run_event_modify_dialog(
	Node 			*target,
	GtkTreeModel	*tree_model,
	GtkTreeIter		*iter_temp){
	/* used to modify the event node in tree store*/
	GtkWidget	*data_modify_dialog;
	GtkWidget	*entry_event_serial;
	GtkWidget	*entry_event_time;
	GtkWidget	*spinbutton_event_level;
	GtkWidget	*entry_event_location;
	GtkWidget	*spinbutton_event_deads;
	GtkWidget	*spinbutton_event_house;
	GtkWidget	*spinbutton_event_money;
	GtkBuilder	*builder_temp	=	gtk_builder_new();
	/* if the main of a superior node has changed the child of is will be changed too */
	/* in case the text changed when changing the child node */
	char mainkeybuffer[30];
	char detail_buff[200];

	/* show the dialog to modify the node data */
	gtk_builder_add_from_file(builder_temp,"builder.ui",NULL);
	data_modify_dialog		=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"event_modify_dialog"));
	entry_event_serial		=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"entry_event_serial"));
	entry_event_time		=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"entry_event_time"));
	spinbutton_event_level	=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"spinbutton_event_level"));
	entry_event_location	=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"entry_event_location"));
	spinbutton_event_deads	=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"spinbutton_event_deads"));
	spinbutton_event_house	=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"spinbutton_event_house"));
	spinbutton_event_money	=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"spinbutton_event_money"));
	gtk_window_set_transient_for(GTK_WINDOW(data_modify_dialog),GTK_WINDOW(window_main));

	/* set the default text */
	gtk_entry_set_text(GTK_ENTRY(entry_event_serial),serial_event(target));
	gtk_entry_set_text(GTK_ENTRY(entry_event_time),time_event(target));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_event_level),level_event(target)-'0');
	gtk_entry_set_text(GTK_ENTRY(entry_event_location),location_event(target));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_event_deads),deads_event(target));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_event_house),house_destroyed_event(target));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_event_money),pencuniary_loss_event(target));

	/* if apply the change */
	if(gtk_dialog_run(GTK_DIALOG(data_modify_dialog))==1){
		/* get the data & change the data in target node (in chain) */
		strcpy(mainkeybuffer,gtk_entry_get_text(GTK_ENTRY(entry_event_serial)));
		strcpy(serial_event(target),mainkeybuffer);
		strcpy(time_event(target),gtk_entry_get_text(GTK_ENTRY(entry_event_time)));
		strcpy(location_event(target),gtk_entry_get_text(GTK_ENTRY(entry_event_location)));
		level_event(target)				=	gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbutton_event_level))+'0';
		deads_event(target)				=	gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbutton_event_deads));
		house_destroyed_event(target)	=	gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbutton_event_house));
		pencuniary_loss_event(target)	=	gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbutton_event_money));

		/*  change the child data in chain */
		Node *temp1;	/* donate changer */
		for(temp1=target->child;temp1!=NULL;temp1=temp1->next)
			strcpy(serial_donate(temp1),mainkeybuffer);
		/* change the presentation in tree view */
			sprintf(detail_buff,
				"serial:%-13s time:%-16s level:%c location:%-21s "
				"deads:%-6d house destroyed:%-6d pecuniary loss:%.2f",
				serial_event(target),
				time_event(target),
				level_event(target),
				location_event(target),
				deads_event(target),
				house_destroyed_event(target),
				pencuniary_loss_event(target));
			gtk_tree_store_set(GTK_TREE_STORE(tree_model),iter_temp,
				TREE_ALL_NODE_TYPE,			"event",
				TREE_ALL_CLASSIFY_TYPE,		"",
				TREE_ALL_CLASSIFY_TYPE_NUM,	0,
				TREE_ALL_CLASSIFY_NAME,		"",
				TREE_ALL_EVENT_SERIAL,		serial_event(target),
				TREE_ALL_DONATE_NAME,		"",
				TREE_ALL_DETAIL_BUFF,		detail_buff,
				TREE_ALL_NODE_ADDRESS,			target,-1);
	}
	g_object_unref(G_OBJECT(builder_temp));
	gtk_widget_destroy(GTK_WIDGET(data_modify_dialog));
	free(iter_temp);
}

void run_donate_modify_dialog(
	Node 			*target,
	GtkTreeModel	*tree_model,
	GtkTreeIter		*iter_temp){
	/* used to modify the donate node in tree store*/
	GtkWidget	*data_modify_dialog;
	GtkWidget	*entry_donate_name;
	GtkWidget	*comboboxtext_donate_type;
	GtkWidget	*entry_donate_time;
	GtkWidget	*spinbutton_donate_amount;
	GtkBuilder	*builder_temp	=	gtk_builder_new();
	/* if the main of a superior node has changed the child of is will be changed too */
	/* in case the text changed when changing the child node */
	char detail_buff[200];

	/* show the dialog to modify the node data */
	gtk_builder_add_from_file(builder_temp,"builder.ui",NULL);
	data_modify_dialog		=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"donate_modify_dialog"));
	entry_donate_name			=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"entry_donate_name"));
	comboboxtext_donate_type	=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"comboboxtext_donate_type"));
	entry_donate_time			=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"entry_donate_time"));
	spinbutton_donate_amount	=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"spinbutton_donate_amount"));
	gtk_window_set_transient_for(GTK_WINDOW(data_modify_dialog),GTK_WINDOW(window_main));

	/* set the default text */
	gtk_entry_set_text(GTK_ENTRY(entry_donate_name),name_donate(target));
	gtk_entry_set_text(GTK_ENTRY(entry_donate_time),time_donate(target));
	gtk_combo_box_set_active(GTK_COMBO_BOX(comboboxtext_donate_type),type_donate(target)-'1');
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_donate_amount),amount_donate(target));

	/* if apply the change */
	if(gtk_dialog_run(GTK_DIALOG(data_modify_dialog))==1){
		/* get the data & change the data in target node (in chain) */
		strcpy(name_donate(target),gtk_entry_get_text(GTK_ENTRY(entry_donate_name)));
		strcpy(time_donate(target),gtk_entry_get_text(GTK_ENTRY(entry_donate_time)));
		amount_donate(target)	=	gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbutton_donate_amount));
		type_donate(target)		=	gtk_combo_box_get_active(GTK_COMBO_BOX(comboboxtext_donate_type))+'1';
		/* change the presentation in tree view */
		sprintf(detail_buff,
			"time:%-16s donate type:%s donor's name:%-21s donate amount:%.2f",
			time_donate(target),
			string_type_donate(type_donate(target)),
			name_donate(target),
			amount_donate(target));
		gtk_tree_store_set(GTK_TREE_STORE(tree_model),iter_temp,
			TREE_ALL_NODE_TYPE,			"donate",
			TREE_ALL_CLASSIFY_TYPE,		"",
			TREE_ALL_CLASSIFY_TYPE_NUM,	0,
			TREE_ALL_CLASSIFY_NAME,		"",
			TREE_ALL_EVENT_SERIAL,		"",
			TREE_ALL_DONATE_NAME,		name_donate(target),
			TREE_ALL_DETAIL_BUFF,		detail_buff,
			TREE_ALL_NODE_ADDRESS,		target,-1);
	}
	g_object_unref(G_OBJECT(builder_temp));
	gtk_widget_destroy(GTK_WIDGET(data_modify_dialog));
	free(iter_temp);
}


