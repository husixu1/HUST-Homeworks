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
#include "treeview_all.h"
#include "treeview_all_modify.h"
#include "treeview_all_adddata.h"
#include "data_structure.h"
#include <gtk.h>
#include <stdlib.h>

/** add the row right-click function */


void treeview_all_init(GtkEntry *search_entry){
	/* set the default search entry */
	gtk_tree_view_set_search_entry(GTK_TREE_VIEW(treeview_main),search_entry);
	/* set the vague search function */
	gtk_tree_view_set_search_equal_func(
		GTK_TREE_VIEW(treeview_main),
		(GtkTreeViewSearchEqualFunc)__mid_search_func,
		gtk_tree_view_get_model(GTK_TREE_VIEW(treeview_main)), NULL);
}

/* reload the tree view (used after inserted a data independently) */
void treeview_all_reload(void){
    GtkTreeStore	*treestore	=	GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview_main)));
    g_object_ref(G_OBJECT(treestore));
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_main),NULL);
    gtk_tree_store_clear(treestore);
    treeview_add_all_data(treestore);
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_main),GTK_TREE_MODEL(treestore));
	if(is_treeview_main_expanded)
		gtk_tree_view_expand_all(GTK_TREE_VIEW(treeview_main));
}
/* double-click on a row, asking if modify the data(Node)*/
G_MODULE_EXPORT void on_treeview_all_row_activated(GtkTreeSelection *selection){
	GtkTreeIter			*iter_temp	=	(GtkTreeIter *)malloc(sizeof(GtkTreeIter));
    GtkTreeModel		*tree_model_temp;

    /* judge if really selected a row */
	gtk_tree_selection_get_selected(selection,&tree_model_temp,iter_temp);
	if(iter_temp==NULL){
		free(iter_temp);
		return;
	}

	/* retrieve the node */
	Node *target;
	gtk_tree_model_get(tree_model_temp,iter_temp,TREE_ALL_NODE_ADDRESS,&target,-1);

	switch(target->node_type){
		case node_type_classify:
			run_classify_modify_dialog(target,tree_model_temp,iter_temp);
			break;
		case node_type_event:
			run_event_modify_dialog(target,tree_model_temp,iter_temp);
			break;
		case node_type_donate:
			run_donate_modify_dialog(target,tree_model_temp,iter_temp);
			break;
	}
}

G_MODULE_EXPORT void on_treeview_all_button_release_event(GtkTreeSelection *selection,GdkEvent *event){
	selection	=	selection;
	/* is right click */
	if(event->button.button==3)
		gtk_menu_popup(GTK_MENU(menu_rightclick),NULL,NULL,NULL,NULL,3,gtk_get_current_event_time());
}

G_MODULE_EXPORT void on_treeviewcolumn_clicked(GtkTreeViewColumn *column){

    GtkTreeView *treeview_temp	=	GTK_TREE_VIEW(gtk_tree_view_column_get_tree_view(column));
    unsigned int i=0;
    for(;i<gtk_tree_view_get_n_columns(treeview_temp);i++)
		if(gtk_tree_view_get_column(treeview_temp,i)	==	column)
			break;

	/* convert i to the column in the tree model (when i>=2,++i) */
    gtk_tree_view_set_search_column(treeview_temp,(i>1)?++i:i);
}

/* the internal search function */
gboolean __mid_search_func(
	GtkTreeModel *model, gint column,
	const gchar *key, GtkTreeIter *iter)
{
    gchar *iter_string = NULL;
    gtk_tree_model_get(model, iter, column, &iter_string, -1);
    return g_strrstr(iter_string, key) == NULL;
}
