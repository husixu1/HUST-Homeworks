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

#ifndef NODE_OPERATION_H_INCLUDED
#define NODE_OPERATION_H_INCLUDED

#include "data_structure.h"


/* initialize the head nodes	*/
/* returning: no_error			*/
error_t head_init(void);

/* delete the whole chain 	*/
/* returning: no_error 		*/
error_t node_delete_all(void);

/* initialize a node 						*/
/* returning: no_error, error_malloc 		*/
error_t node_init(Node **target, node_type_t type);

/* insert a node (assume malloced already) 					*/
/* node.type is required, all the data is required 			*/
/* returning: no_error, error_parameter, error_no_parent 	*/
error_t node_insert(Node *target);

/* delete a node 										*/
/* returning: error_parameter error_had_child, no_error */
error_t node_delete(Node *target);

/* recursively delete a node 		 	*/
/* returning: error_parameter, no_error */
error_t node_delete_recursive(Node *target);

/* try to find a node according to a known criteria 							*/
/* search range from keyword_s to keyword_e, and the sequence doesn't matter 	*/
error_t node_cri_inquire(criteria_t criteria, Node *head, char *keyword_s, char *keyword_e);

/* internal function, ignore this */
error_t __node_clear_pointer(Node *target);

#endif // NODE_OPERATION_H_INCLUDED
