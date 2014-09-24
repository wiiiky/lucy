/*
 * lccommander.c
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
#include "lccommander.h"
#include "lcsocket.h"
#include "lcutil.h"


static GList *sockets = NULL;

static gboolean lc_socket_is_closed_wrapper(gconstpointer data,
                                            gpointer user_data)
{
    LcSocket *s = LC_SOCKET(data);
    return lc_socket_is_closed(s);
}

static GList *lc_commander_clean_sockets()
{
    sockets =
        lc_util_clean_list_by(sockets, lc_socket_is_closed_wrapper, NULL,
                              g_object_unref);
    return sockets;
}

static LcSocket *lc_commander_find_idle_socket()
{
    GList *lp = lc_commander_clean_sockets();
    while (lp) {
        LcSocket *s = (LcSocket *) lp->data;
        if (!lc_socket_is_busy(s)) {
            return s;
        }
        lp = g_list_next(lp);
    }
    return NULL;
}


void lc_commander_send_command(const gchar * cmd,
                               GAsyncReadyCallback callback, gpointer data)
{
    LcSocket *s = lc_commander_find_idle_socket();
    /* TODO */
    if (s == NULL) {
        g_message("No Idle Socket is found,creating a new one!");
    }
}
