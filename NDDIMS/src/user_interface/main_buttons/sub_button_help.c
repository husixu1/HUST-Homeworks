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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtk.h>
#include <string.h>

/* the about dialog */
G_MODULE_EXPORT void on_button_help_about_clicked(void){
	GtkBuilder	*builder_dialog	=	gtk_builder_new();
	gtk_builder_add_from_file(builder_dialog,"dialogs.ui",NULL);
	gtk_builder_connect_signals(builder_dialog,NULL);

	GtkDialog*	about	=	GTK_DIALOG(gtk_builder_get_object(builder_dialog,"aboutdialog1"));
	gtk_window_set_transient_for(GTK_WINDOW(about),GTK_WINDOW(window_main));
	
	gtk_dialog_run(about);
	gtk_widget_destroy(GTK_WIDGET(about));

	g_object_unref(G_OBJECT(builder_dialog));
}

G_MODULE_EXPORT void on_button_help_help_clicked(void){
	GtkBuilder	*builder_dialog	=	gtk_builder_new();
	gtk_builder_add_from_file(builder_dialog,"dialogs.ui",NULL);
	gtk_builder_connect_signals(builder_dialog,NULL);

	GtkDialog*	about	=	GTK_DIALOG(gtk_builder_get_object(builder_dialog,"dialog_help"));
	gtk_window_set_transient_for(GTK_WINDOW(about),GTK_WINDOW(window_main));
	gtk_dialog_run(about);

	g_object_unref(G_OBJECT(builder_dialog));
}

G_MODULE_EXPORT	void on_aboutdialog1_activate_link(GtkAboutDialog	*label,gchar	*uri){
	label	=	label;
	char cmd[200]="chromium ";
	strcat(cmd,uri);
	system(cmd);
}
