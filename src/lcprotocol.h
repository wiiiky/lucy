/*
 * lcprotocol.h
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

#ifndef __LC_DATA_MODEL_H__
#define __LC_DATA_MODEL_H__
#include <glib-2.0/glib.h>

typedef enum {
    LC_PROTOCOL_RESULT_OKAY,
    LC_PROTOCOL_RESULT_FAIL,
} LcProtocolResult;

typedef struct {
    gchar *packageName;
    gchar *appName;
    gchar *version;
    gchar *iconId;
    gchar *installedTime;
    gchar *installedLocation;
    gchar *description;
} LcProtocolApplication;

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
                                                   description);
void lc_protocol_application_free(LcProtocolApplication * p);

/*
 * 从一个列表中找出包名为package的项，未找到返回NULL
 */
LcProtocolApplication *lc_protocol_application_find(GList * list,
                                                    const gchar * package);


#endif
