#ifndef FILE_OPERATION_H_INCLUDED
#define FILE_OPERATION_H_INCLUDED

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

#include <stdio.h>
#include "data_structure.h"

/*	final file save structure
*	|Node|data|Node|data|...|Node|data|EOF|
*	from the |Node| area you can know which type of node it is
*/

#define MAX_FILENAME_LEN	1000
extern char file_current_name[MAX_FILENAME_LEN];
/* Save the whole chain in a file */
error_t file_save(const char *file_name);
/* Load the whole chain from file*/
error_t file_load(const char *file_name);

#define MAX_DATA_SIZE sizeof(Event)

#endif // FILE_OPERATION_H_INCLUDED

