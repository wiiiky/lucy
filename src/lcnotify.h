/*
 * lcnotify.h
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

#ifndef __LC_NOTIFY_H__
#define __LC_NOTIFY_H__

#include <glib.h>

void lc_notify_show_full(const gchar * summary, const gchar * body,
                         const gchar * icon);

/*
 * show a notification message with default icon
 */
void lc_notify_show(const gchar * summary, const gchar * body);


#endif
