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
#include "treeview_all_adddata.h"
#include "file_operation.h"
#include "file_operation_xml.h"
#include "node_operation.h"
#include "data_structure.h"
#include "display.h"
#include <gtk.h>
#include <string.h>
#include <stdlib.h>

G_MODULE_EXPORT void on_button_file_system_new_clicked(void){
	GtkBuilder	*builder_temp_dialog	=	gtk_builder_new();
	gtk_builder_add_from_file(builder_temp_dialog,"dialogs.ui",NULL);
	gtk_builder_connect_signals(builder_temp_dialog,NULL);
	/* if already opened a file */
	if(file_current_name[0]){
		GtkDialog	*dialog_confirm	=	GTK_DIALOG(gtk_builder_get_object(builder_temp_dialog,"dialog_open_confirm"));
		gtk_window_set_transient_for(GTK_WINDOW(dialog_confirm),GTK_WINDOW(window_main));
		/* if choose not to override */
		if(gtk_dialog_run(dialog_confirm) == 0){
			file_save(file_current_name);
			goto end;
		}
		GtkWidget		*tree_view	=	treeview_main;
        GtkTreeStore 	*tree_store	=	GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view)));
		gtk_widget_destroy(GTK_WIDGET(dialog_confirm));
		/* override */
		node_delete_all();
		gtk_tree_store_clear(tree_store);
	}
	/* ask where to save the new file */
	GtkWidget	*file_chooser_dialog	=	GTK_WIDGET(gtk_builder_get_object(builder_temp_dialog,"file_save_as_dialog"));
	gtk_window_set_transient_for(GTK_WINDOW(file_chooser_dialog),GTK_WINDOW(window_main));
	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(file_chooser_dialog),"Untitled.dat");
	if(gtk_dialog_run(GTK_DIALOG(file_chooser_dialog))==1){
			char 	*file_name	=	gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_dialog));
			file_save(file_name);
			/* set the title to the current name */
			char title[60];
			strncat(title,file_name,50);
			gtk_window_set_title(GTK_WINDOW(window_main),strcat(title," - NDDIMS"));
			/* change the current filename to this */
			strcpy(file_current_name,file_name);
			g_free(file_name);
	}
	gtk_widget_destroy(file_chooser_dialog);
end:
	g_object_unref(G_OBJECT(builder_temp_dialog));
	__restore_menu_toggle_button();
}

G_MODULE_EXPORT void on_button_file_system_open_clicked(void){
	GtkBuilder	*builder_temp_dialog	=	gtk_builder_new();
	GtkWidget	*file_chooser_dialog;

	gtk_builder_add_from_file(builder_temp_dialog,"dialogs.ui",NULL);
	gtk_builder_connect_signals(builder_temp_dialog,NULL);
	file_chooser_dialog	=	GTK_WIDGET(gtk_builder_get_object(builder_temp_dialog,"file_open_dialog"));
	gtk_window_set_transient_for(GTK_WINDOW(file_chooser_dialog),GTK_WINDOW(window_main));
	if(gtk_dialog_run(GTK_DIALOG(file_chooser_dialog))==1){
        char 		*file_name;
        GtkWidget	*tree_view;
        GtkTreeStore 	*tree_store;

        file_name	=	gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_dialog));
        /*** the tree view must always be the first of the third one in the box ***/
        tree_view	=	treeview_main;
		tree_store	=	GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view)));
		/* if already opened a file */
		if(file_current_name[0]){
			GtkDialog	*dialog_confirm	=	GTK_DIALOG(gtk_builder_get_object(builder_temp_dialog,"dialog_open_confirm"));
			/* if choose not to override */
			if(gtk_dialog_run(dialog_confirm) == 0){
				file_save(file_current_name);
				goto end;
			}
			gtk_widget_destroy(GTK_WIDGET(dialog_confirm));
			/* override */
            node_delete_all();
            gtk_tree_store_clear(tree_store);
		}
		file_load(file_name);
		/* set the title to the current name */
		char title[61];
		strncat(title,file_name,50);
		gtk_window_set_title(GTK_WINDOW(window_main),strcat(title," - NDDIMS"));
		g_free(file_name);
		/* in order to speedup the process */
		g_object_ref(tree_store);
		gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view),NULL);
		treeview_add_all_data(tree_store);
		gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view),GTK_TREE_MODEL(tree_store));
	}
end:
	g_object_unref(G_OBJECT(builder_temp_dialog));
	gtk_widget_destroy(file_chooser_dialog);
	__restore_menu_toggle_button();
}

G_MODULE_EXPORT void on_button_file_system_save_clicked(void){
	/* judge if */
	if(file_current_name[0])
		file_save(file_current_name);
	else{
		/* the same as on_button_file_system_save_as_clicked */
		/* but you can't call a G_MODULE_EXPORT function manually */
		GtkBuilder	*builder_temp_dialog	=	gtk_builder_new();
		GtkWidget	*file_chooser_dialog;

		gtk_builder_add_from_file(builder_temp_dialog,"dialogs.ui",NULL);
		gtk_builder_connect_signals(builder_temp_dialog,NULL);
		file_chooser_dialog	=	GTK_WIDGET(gtk_builder_get_object(builder_temp_dialog,"file_save_as_dialog"));
		/* set the default filename */
		char 	file_name[1000];
		if(file_current_name[0]){
			strcpy(file_name,file_current_name);
			/* get the filename from the path */
			char *a;
			for(a=&file_name[strlen(file_name)]; a!=file_name&&*a!='/'&&*a!='\\'; a--);
			strcpy(file_name, (*a=='\\' || *a=='/')?++a:a);
		}
		else
			strcpy(file_name,"Untitled.xml");
		gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(file_chooser_dialog),file_name);
		gtk_window_set_transient_for(GTK_WINDOW(file_chooser_dialog),GTK_WINDOW(window_main));
		if(gtk_dialog_run(GTK_DIALOG(file_chooser_dialog))==1){
				strcpy(file_name, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_dialog)));
				/* make sure the file format is .dat */
				if(	(strstr(file_name,".dat")==NULL)	||
					(strstr(file_name,".dat")-file_name)!=((int)strlen(file_name)-4) )
					strcat(file_name,".dat");
				file_save(file_name);
				/* set the title to the current name */
				char title[60];
				strncat(title,file_name,50);
				gtk_window_set_title(GTK_WINDOW(window_main),strcat(title," - NDDIMS"));
				/* change the current filename to this */
				strcpy(file_current_name,file_name);
		}
		g_object_unref(G_OBJECT(builder_temp_dialog));
		gtk_widget_destroy(file_chooser_dialog);
		__restore_menu_toggle_button();
	}
	__restore_menu_toggle_button();
}

G_MODULE_EXPORT void on_button_file_system_save_as_clicked(void){
	GtkBuilder	*builder_temp_dialog	=	gtk_builder_new();
	GtkWidget	*file_chooser_dialog;

	gtk_builder_add_from_file(builder_temp_dialog,"dialogs.ui",NULL);
	gtk_builder_connect_signals(builder_temp_dialog,NULL);
	file_chooser_dialog	=	GTK_WIDGET(gtk_builder_get_object(builder_temp_dialog,"file_save_as_dialog"));
	/* set the default filename */
	char 	file_name[1000];
	if(file_current_name[0]){
		strcpy(file_name,file_current_name);
		/* get the filename from the path */
		char *a;
		for(a=&file_name[strlen(file_name)]; a!=file_name&&*a!='/'&&*a!='\\'; a--);
		strcpy(file_name, (*a=='\\' || *a=='/')?++a:a);
	}
	else
		strcpy(file_name,"Untitled.dat");
	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(file_chooser_dialog),file_name);
	gtk_window_set_transient_for(GTK_WINDOW(file_chooser_dialog),GTK_WINDOW(window_main));
	if(gtk_dialog_run(GTK_DIALOG(file_chooser_dialog))==1){
			strcpy(file_name, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_dialog)));
			/* make sure the file format is .dat */
			if(	(strstr(file_name,".dat")==NULL)	||
				(strstr(file_name,".dat")-file_name)!=((int)strlen(file_name)-4) )
				strcat(file_name,".dat");
			file_save(file_name);
			/* set the title to the current name */
			char title[60];
			strncat(title,file_name,50);
			gtk_window_set_title(GTK_WINDOW(window_main),strcat(title," - NDDIMS"));
			/* change the current filename to this */
			strcpy(file_current_name,file_name);
	}
	g_object_unref(G_OBJECT(builder_temp_dialog));
	gtk_widget_destroy(file_chooser_dialog);
	__restore_menu_toggle_button();
}

G_MODULE_EXPORT void on_button_file_system_export_clicked(void){
	GtkBuilder	*builder_temp_dialog	=	gtk_builder_new();
	GtkWidget	*file_chooser_dialog;

	gtk_builder_add_from_file(builder_temp_dialog,"dialogs.ui",NULL);
	gtk_builder_connect_signals(builder_temp_dialog,NULL);
	file_chooser_dialog	=	GTK_WIDGET(gtk_builder_get_object(builder_temp_dialog,"file_save_as_dialog"));
	char 	file_name[1000];
	/* set the default filename */
	if(file_current_name[0]){
		strcpy(file_name,file_current_name);
		/* get the filename from the path */
		char *a;
		for(a=&file_name[strlen(file_name)]; a!=file_name&&*a!='/'&&*a!='\\'; a--);
		strcpy(file_name, (*a=='\\' || *a=='/')?++a:a);
	}
	else
		strcpy(file_name,"Untitled.xml");
	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(file_chooser_dialog), file_name);
	gtk_window_set_transient_for(GTK_WINDOW(file_chooser_dialog),GTK_WINDOW(window_main));
	if(gtk_dialog_run(GTK_DIALOG(file_chooser_dialog))==1){
		/* make sure the file format is .xml*/
		strcpy(file_name, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_dialog)));
		if(	(strstr(file_name,".xml")==NULL)	||
			(strstr(file_name,".xml")-file_name)!=((int)strlen(file_name)-4) )
			strcat(file_name,".xml");
		file_save_as_xml(file_name);
	}
	g_object_unref(G_OBJECT(builder_temp_dialog));
	gtk_widget_destroy(file_chooser_dialog);
	__restore_menu_toggle_button();
}

