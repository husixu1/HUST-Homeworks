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

#include "main.h"
#include "file_operation.h"
#include "node_operation.h"
#include "data_structure.h"
#include "display.h"
#include "__random_data_generate.h"
#include "__developer_tools.h"
#include <gtk/gtk.h>
#include <stdio.h>



int main(int argc, char *argv[]){
	head_init();
	display(argc,argv);
//	__generate_random_data(100,10,10);
//	__develpoer_mode_init();
	return 0;
}

