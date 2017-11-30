#ifndef TREEVIEW_ALL_MODIFY_H_INCLUDED
#define TREEVIEW_ALL_MODIFY_H_INCLUDED

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

#include "data_structure.h"
#include "gtk.h"
/* run three sub dialogs */
void run_classify_modify_dialog(
	Node			*target,
	GtkTreeModel	*tree_model,
	GtkTreeIter		*tree_iter);
void run_event_modify_dialog(
	Node			*target,
	GtkTreeModel	*tree_model,
	GtkTreeIter		*tree_iter);
void run_donate_modify_dialog(
	Node			*target,
	GtkTreeModel	*tree_model,
	GtkTreeIter		*tree_iter);

#endif // TREEVIEW_ALL_MODIFY_H_INCLUDED
