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
#include "treeview_all_adddata.h"
#include "treeview_all.h"
#include "file_operation.h"
#include <gtk.h>
#include <stdio.h>

/* the outline frame inside the main window */
GtkWidget	*window_main;
GtkWidget	*box_main;
GtkWidget	*treeview_main;
/* the searching windows */
GtkListStore	*liststore_search_result;
GtkWidget		*menu_rightclick;

gboolean is_treeview_main_expanded	=	0;

static void __init(void);

int display(int argc, char **argv){

	gtk_init(&argc,&argv);

	GtkApplication	*app;
	int status;
	app	=	gtk_application_new("com.github.husixu1.example",G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app,"activate",G_CALLBACK(__init),NULL);
	status	=	g_application_run(G_APPLICATION(app),argc,argv);
	g_object_unref(app);

	return status;
}

static void __init(void)
{
	GtkBuilder	*builder_builder	=	gtk_builder_new();
	GtkBuilder	*builder_searching	=	gtk_builder_new();

	gtk_builder_add_from_file(builder_builder,"builder.ui",NULL);
	gtk_builder_add_from_file(builder_searching,"searching.ui",NULL);
	gtk_builder_connect_signals(builder_builder,NULL);
	gtk_builder_connect_signals(builder_searching,NULL);

	window_main		=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"NDDIMS"));
	box_main		=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"box2"));
	treeview_main	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"treeview_all"));
	liststore_search_result	=	GTK_LIST_STORE(gtk_builder_get_object(builder_searching,"liststore_search_result"));
	menu_rightclick	=	GTK_WIDGET(gtk_builder_get_object(builder_builder,"menu_tree_all_rightclick"));

	g_object_ref(G_OBJECT(liststore_search_result));
	g_object_ref(G_OBJECT(menu_rightclick));
	/* set the search entry properly */
	GtkEntry	*search_entry		=	GTK_ENTRY(gtk_builder_get_object(builder_builder,"treeview_all_search_entry"));
	treeview_all_init(search_entry);

	g_object_unref(G_OBJECT(builder_builder));
	g_object_unref(G_OBJECT(builder_searching));
	gtk_widget_show(window_main);

	gtk_main();
}

G_MODULE_EXPORT gboolean on_NDDIMS_destroy(void){
	if(file_current_name[0]=='\0'){
		gtk_widget_destroy(window_main);
		gtk_main_quit();
		return FALSE;
	}
    GtkBuilder	*builder_dialog	=	gtk_builder_new();
	gtk_builder_add_from_file(builder_dialog, "dialogs.ui", NULL);
	gtk_builder_connect_signals(builder_dialog,NULL);

	GtkDialog *dialog_close_confirm	=	GTK_DIALOG(gtk_builder_get_object(builder_dialog,"dialog_close_confirm"));
	gtk_window_set_transient_for(GTK_WINDOW(dialog_close_confirm),GTK_WINDOW(window_main));
	switch(gtk_dialog_run(dialog_close_confirm)){
		/* YES,save and exit */
		case 1:
			file_save(file_current_name);
			gtk_widget_destroy(window_main);
			gtk_main_quit();
			return FALSE;
		/* NO, Do not save and exit*/
		case 2:
			gtk_widget_destroy(window_main);
			gtk_main_quit();
			return FALSE;
		/* CANCEL, continue editing */
		case 3:
		default:
			gtk_widget_destroy(GTK_WIDGET(dialog_close_confirm));
			return TRUE;
	}
}
