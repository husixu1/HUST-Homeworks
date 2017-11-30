#ifndef CHAIN_OPERATION_H_INCLUDED
#define CHAIN_OPERATION_H_INCLUDED

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

/* after each search , clear the search node */
/* search(vague) the classify node according to name */

/* search for the classify node , the result is stored in the inquire_result */
error_t chain_inquire_classify(const char type,const char *keyword);

/* search for the event node */
error_t chain_inquire_event(
	const char	*serial_start,	const char	*serial_end,
	const char	*name_key,
	const char	*time_start,	const char	*time_end,
	const int	level_start,	const int	level_end,
	const int	deads_start,	const int	deads_end,
	const int	house_start,	const int	house_end,
	const float	money_start,	const float	money_end);

/* search for the donate node */
error_t chain_inquire_donate(
	const char	*name_key,
	const char	*serial_start,	const char	*serial_end,
	const char	*time_start,	const char	*time_end,
	const float	amount_start,	const float	amount_end,
	const char	*classify_name,
	const char	type									);

/* do the statistics */
error_t chain_inquire_statistic_event(const char type, int level_start, int level_end,const char *name);

#endif // CHAIN_OPERATION_H_INCLUDED
