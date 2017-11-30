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
#include "treeview_search_result.h"
#include <gtk.h>

void search_result_show_window(void){
	GtkWidget	*window_search_result;
	GtkTreeView	*treeview_search_result;
	GtkBuilder	*builder_searching	=	gtk_builder_new();
	gtk_builder_add_from_file(builder_searching,"searching.ui",NULL);
	gtk_builder_connect_signals(builder_searching,NULL);

	treeview_search_result	=	GTK_TREE_VIEW(gtk_builder_get_object(builder_searching,"treeview_search_result"));
	window_search_result	=	GTK_WIDGET(gtk_builder_get_object(builder_searching,"window_searching_result"));
	gtk_tree_view_set_model(treeview_search_result,GTK_TREE_MODEL(liststore_search_result));
	gtk_window_set_transient_for(GTK_WINDOW(window_search_result),GTK_WINDOW(window_main));
    gtk_widget_show_all(GTK_WIDGET(window_search_result));

    g_object_unref(G_OBJECT(builder_searching));
}

G_MODULE_EXPORT void on_treeview_search_result_row_activated(void){

}
