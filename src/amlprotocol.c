/*
 * amlprotocol.c
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
#include "amlprotocol.h"

AmlProtocolPackage *aml_protocol_package_new(const gchar * packageName,
                                             const gchar * appName,
                                             const gchar * version,
                                             const gchar * iconId,
                                             const gchar * installedTime,
                                             const gchar *
                                             installedLocation,
                                             const gchar * description)
{
    AmlProtocolPackage *p =
        (AmlProtocolPackage *) g_slice_alloc0(sizeof(AmlProtocolPackage));
    p->packageName = g_strdup(packageName);
    p->appName = g_strdup(appName);
    p->version = g_strdup(version);
    p->iconId = g_strdup(iconId);
    p->installedTime = g_strdup(installedTime);
    p->installedLocation = g_strdup(installedLocation);
    p->description = g_strdup(description);
    return p;
}

void aml_protocol_package_free(AmlProtocolPackage * p)
{
    if (p == NULL) {
        return;
    }
    g_free(p->packageName);
    g_free(p->appName);
    g_free(p->version);
    g_free(p->iconId);
    g_free(p->installedTime);
    g_free(p->installedLocation);
    g_free(p->description);
    g_slice_free1(sizeof(AmlProtocolPackage), p);
}
