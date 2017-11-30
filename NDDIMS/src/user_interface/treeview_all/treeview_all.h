#ifndef TREEVIEW_ALL_H_INCLUDED
#define TREEVIEW_ALL_H_INCLUDED

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
#include <gtk.h>

/* init the default treeview */
void treeview_all_init(GtkEntry *search_entry);

G_MODULE_EXPORT void on_treeview_all_row_activated(GtkTreeSelection *selection);

/* reload the tree view according to the current node */
void treeview_all_reload(void);
/* the internal search function */
gboolean __mid_search_func(
	GtkTreeModel *model, gint column,
	const gchar *key, GtkTreeIter *iter);

#endif // USER_INTERFACE_H_INCLUDED
