/*
 * lcprotocol.c
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
#include "lcprotocol.h"


LcProtocolResult lc_protocol_get_result_from_string(const gchar * str)
{
    if (g_ascii_tolower(str[0]) == 'o' &&
        g_ascii_tolower(str[1]) == 'k' &&
        g_ascii_tolower(str[2]) == 'a' && g_ascii_tolower(str[3]) == 'y') {
        return LC_PROTOCOL_RESULT_OKAY;
    }
    return LC_PROTOCOL_RESULT_FAIL;
}


LcProtocolApplication *lc_protocol_application_new(const gchar *
                                                   packageName,
                                                   const gchar * appName,
                                                   const gchar * version,
                                                   const gchar * iconId,
                                                   const gchar *
                                                   installedTime,
                                                   const gchar *
                                                   installedLocation,
                                                   const gchar *
                                                   description)
{
    LcProtocolApplication *p = (LcProtocolApplication *)
        g_slice_alloc0(sizeof(LcProtocolApplication));
    p->packageName = g_strdup(packageName);
    p->appName = g_strdup(appName);
    p->version = g_strdup(version);
    p->iconId = g_strdup(iconId);
    p->installedTime = g_strdup(installedTime);
    p->installedLocation = g_strdup(installedLocation);
    p->description = g_strdup(description);
    return p;
}

static inline void
lc_protocol_application_free_internal(LcProtocolApplication * p)
{
    g_free(p->packageName);
    g_free(p->appName);
    g_free(p->version);
    g_free(p->iconId);
    g_free(p->installedTime);
    g_free(p->installedLocation);
    g_free(p->description);
}

void lc_protocol_application_free(LcProtocolApplication * p)
{
    if (p == NULL) {
        return;
    }
    lc_protocol_application_free_internal(p);
    g_slice_free1(sizeof(LcProtocolApplication), p);
}

LcProtocolApplication *lc_protocol_application_find(GList * list,
                                                    const gchar * package)
{
    while (list) {
        LcProtocolApplication *app = (LcProtocolApplication *) list->data;
        if (g_strcmp0(app->packageName, package) == 0) {
            return app;
        }
        list = g_list_next(list);
    }
    return NULL;
}

LcProtocolApplication *lc_protocol_get_application(const gchar * data)
{
    gchar **elements = g_strsplit(data, ":", -1);
    if (g_strv_length(elements) < 7) {
        g_strfreev(elements);
        return NULL;
    }
    LcProtocolApplication *app = lc_protocol_application_new(elements[0],
                                                             elements[1],
                                                             elements[2],
                                                             elements[3],
                                                             elements[4],
                                                             elements[5],
                                                             elements[6]);
    g_strfreev(elements);
    return app;
}

GList *lc_protocol_create_application_list(const gchar * data)
{
    if (data == NULL) {
        return NULL;
    }
    GList *apps = NULL;
    gchar **strarray = g_strsplit(data, "\n", -1);
    gint i = 0;
    while (strarray[i]) {
        gchar *line = strarray[i];
        if (line[0]) {
            LcProtocolApplication *app = lc_protocol_get_application(line);
            apps = g_list_append(apps, app);
        }
        i++;
    }
    g_strfreev(strarray);
    return apps;
}

void lc_protocol_free_application_list(GList * list)
{
    g_list_free_full(list, (GDestroyNotify) lc_protocol_application_free);
}
