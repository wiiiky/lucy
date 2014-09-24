/*
 * lcutil.h
 *
 * Copyright (C) 2014 - Wiky L
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __LC_UTIL_H__
#define __LC_UTIL_H__

#include <glib-2.0/glib.h>
#include "lcprotocol.h"

/*
 * 从字节数组中提取出字符串，添加末尾\0
 */
gchar *lc_util_get_string_from_byte_array(GByteArray * array,
                                          gsize * size);


#endif
