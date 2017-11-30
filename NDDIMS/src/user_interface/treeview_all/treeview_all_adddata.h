#ifndef TREEVIEW_ALL_ADDDATA_H_INCLUDED
#define TREEVIEW_ALL_ADDDATA_H_INCLUDED

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

#include <gtk/gtk.h>

/* convert the classify type to string */
#define	string_type_classify(x)	  \
(x=='1')?"atmosphere	disaster":\
(x=='2')?"ocean			disaster":\
(x=='3')?"flood			disaster":\
(x=='4')?"geological	disaster":\
(x=='5')?"earthquake	disaster":\
(x=='6')?"agriculture	disaster":\
(x=='7')?"forest		disaster":\
		 "Unknown		disaster"
/* convert the donate type to string */
#define string_type_donate(x)	 \
(x=='1')?"personal"				:\
(x=='2')?"enterprise"			:\
		 "Unknown"

#define MAX_DETAIL_BUF_SIZE		1000

#define TREE_ALL_NODE_TYPE			0
#define TREE_ALL_CLASSIFY_TYPE		1
#define TREE_ALL_CLASSIFY_TYPE_NUM	2
#define TREE_ALL_CLASSIFY_NAME		3
#define TREE_ALL_EVENT_SERIAL		4
#define	TREE_ALL_DONATE_NAME		5
#define TREE_ALL_DETAIL_BUFF		6
#define	TREE_ALL_NODE_ADDRESS			7

/* add all the data into the tree store from the chain */
void treeview_add_all_data(GtkTreeStore *tree);

#endif // DATA_OPERATION_H_INCLUDED
