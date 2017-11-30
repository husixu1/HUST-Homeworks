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

#include "main_buttons.h"
#include "sub_button_file_system.h"
#include "display.h"
#include "file_operation.h"
#include <gtk.h>
#include <string.h>

/* collapse and expand the main buttons */
G_MODULE_EXPORT void on_eventbox_collapse_main_button_clicked(GtkWidget *eventbox){
	GtkWidget	*box_main_menu;
	GtkBuilder	*builder_builder;
	switch(gtk_widget_get_tooltip_text(eventbox)[0]){
		/* collapse */
		case 'C':
			box_main_menu	=	gtk_container_get_children(GTK_CONTAINER(box_main))->data;
			gtk_container_remove(GTK_CONTAINER(box_main),box_main_menu);
			/* Change the tool tip */
			gtk_widget_set_tooltip_text(eventbox,"Expand");
			break;
		case 'E':
			builder_builder	=	gtk_builder_new();
			gtk_builder_add_from_file(builder_builder,"builder.ui",NULL);
			gtk_builder_connect_signals(builder_builder,NULL);
			/* re-add the main menu */
			box_main_menu	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"box3"));
			gtk_widget_reparent(box_main_menu,box_main);
			gtk_box_reorder_child(GTK_BOX(box_main),box_main_menu,0);
			g_object_unref(G_OBJECT(builder_builder));
			/* change the tool tip */
			gtk_widget_set_tooltip_text(eventbox,"Collapse");
			break;
	}
}
/* if main button clicked */
G_MODULE_EXPORT void main_button_system_clicked(GtkButton *button){
	/* retreive all the things we need */
	GtkWidget	*second_column		=	(gtk_container_get_children(GTK_CONTAINER(box_main))->next->data);
	GtkBuilder	*builder_temp		=	gtk_builder_new();
	gtk_builder_add_from_file(builder_temp,"submenu.ui",NULL);
	gtk_builder_connect_signals(builder_temp,NULL);

	GtkWidget	*sub_menu;
		 if(!strcmp(gtk_button_get_label(button),"_File System"))
		sub_menu	=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"file_system_sub_menu"));
	else if(!strcmp(gtk_button_get_label(button),"_Data Management"))
		sub_menu	=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"data_management_sub_menu"));
	else if(!strcmp(gtk_button_get_label(button),"_Searching"))
		sub_menu	=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"searching_sub_menu"));
	else if(!strcmp(gtk_button_get_label(button),"S_tatistics"))
		sub_menu	=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"statistics_sub_menu"));
	else
		sub_menu	=	GTK_WIDGET(gtk_builder_get_object(builder_temp,"help_sub_menu"));
	/* remove the old widget and add the new one */
	gtk_container_remove(GTK_CONTAINER(box_main),second_column);
	gtk_container_add(GTK_CONTAINER(box_main),sub_menu);
	gtk_box_reorder_child(GTK_BOX(box_main),sub_menu,1);

	g_object_unref(G_OBJECT(builder_temp));
}

void __restore_menu_toggle_button(void){
	/* destroy the sub-menu */
	gtk_widget_destroy(GTK_WIDGET(gtk_container_get_children(GTK_CONTAINER(box_main))->next->data));
	/* restore the toggle button */
	GtkBuilder	*builder_temp_builder	=	gtk_builder_new();
	gtk_builder_add_from_file(builder_temp_builder,"builder.ui",NULL);
	gtk_builder_connect_signals(builder_temp_builder,NULL);
	GtkWidget	*box_toggle_menu	=	GTK_WIDGET(gtk_builder_get_object(builder_temp_builder,"box1"));
	gtk_widget_reparent(GTK_WIDGET(box_toggle_menu),GTK_WIDGET(box_main));
	gtk_box_reorder_child(GTK_BOX(box_main),GTK_WIDGET(box_toggle_menu),1);

	g_object_unref(G_OBJECT(builder_temp_builder));
}

G_MODULE_EXPORT void on_button_expand_clicked(GtkButton	*button){
	/* expand or collapse the treeview */
	switch(gtk_button_get_label(button)[1]){
		case 'E':
			gtk_tree_view_expand_all(GTK_TREE_VIEW(treeview_main));
			is_treeview_main_expanded	=	1;
			gtk_button_set_label(button,"_Collapse All");
			break;
		case 'C':
			is_treeview_main_expanded	=	0;
			gtk_tree_view_collapse_all(GTK_TREE_VIEW(treeview_main));
			gtk_button_set_label(button,"_Expand All");
			break;
	}
}

G_MODULE_EXPORT void on_button_collapse_clicked(void){
	__restore_menu_toggle_button();
}

