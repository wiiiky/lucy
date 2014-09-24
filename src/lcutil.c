/*
 * lcutil.c
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
#include "lcutil.h"


gchar *lc_util_get_string_from_byte_array(GByteArray * array, gsize * size)
{
    if (array == NULL) {
        return NULL;
    }
    guint8 null = '\0';
    g_byte_array_append(array, &null, 1);
    GBytes *bytes = g_byte_array_free_to_bytes(array);
    gsize len;
    if (size == NULL) {
        size = &len;
    }
    return g_bytes_unref_to_data(bytes, size);
}

GList *lc_util_clean_list_by(GList * list, GListElementNotify judge,
                             gpointer user_data, GDestroyNotify destroy)
{
    GList *lp = list;
    while (lp) {
        GList *next = g_list_next(lp);
        if (judge(lp->data, user_data)) {
            if (lp == list) {   /* first element */
                list = next;
                list->prev = NULL;
                destroy(lp->data);
                g_list_free_1(lp);
            } else {
                lp->prev->next = next;
                next->prev = lp->prev;
                destroy(lp->data);
                g_list_free_1(lp);
            }
        }
        lp = next;
    }
    return list;
}
