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
#include "lcutil.h"
#include <string.h>
#include <stdlib.h>


LcProtocolResult lc_protocol_get_result_from_string(const gchar * str)
{
    if (g_ascii_tolower(str[0]) == 'o' &&
        g_ascii_tolower(str[1]) == 'k' &&
        g_ascii_tolower(str[2]) == 'a' && g_ascii_tolower(str[3]) == 'y') {
        return LC_PROTOCOL_RESULT_OKAY;
    }
    return LC_PROTOCOL_RESULT_FAIL;
}

LcProtocolResult lc_protocol_get_result_from_bytes(GBytes * bytes)
{
    gsize size;
    const gchar *data = (const gchar *) g_bytes_get_data(bytes, &size);
    if (size >= 4 && g_ascii_tolower(data[0]) == 'o' &&
        g_ascii_tolower(data[1]) == 'k' &&
        g_ascii_tolower(data[2]) == 'a' &&
        g_ascii_tolower(data[3]) == 'y') {
        return LC_PROTOCOL_RESULT_OKAY;
    }
    return LC_PROTOCOL_RESULT_FAIL;
}

LcProtocolApplication *lc_protocol_application_new(const gchar *
                                                   package_name,
                                                   const gchar * app_name,
                                                   const gchar * version,
                                                   const gchar *
                                                   installed_time,
                                                   const gchar *
                                                   installed_location,
                                                   const gchar * type,
                                                   const gchar *
                                                   description)
{
    LcProtocolApplication *p = (LcProtocolApplication *)
        g_slice_alloc0(sizeof(LcProtocolApplication));
    p->package_name = g_strdup(package_name);
    p->app_name = g_strdup(app_name);
    p->version = g_strdup(version);
    p->installed_time = g_strdup(installed_time);
    p->location = g_strdup(installed_location);
    if (g_strcmp0(type, "system") == 0) {
        p->type = LC_PROTOCOL_APPLICATION_TYPE_SYSTEM;
    } else if (g_strcmp0(type, "user") == 0) {
        p->type = LC_PROTOCOL_APPLICATION_TYPE_THIRD;
    } else {
        p->type = LC_PROTOCOL_APPLICATION_TYPE_UNKNOWN;
    }
    p->description = g_strdup(description);
    return p;
}

LcProtocolApplication *lc_protocol_application_copy(const
                                                    LcProtocolApplication *
                                                    info)
{
    LcProtocolApplication *p = (LcProtocolApplication *)
        g_slice_alloc0(sizeof(LcProtocolApplication));
    p->package_name = g_strdup(info->package_name);
    p->app_name = g_strdup(info->app_name);
    p->version = g_strdup(info->version);
    p->installed_time = g_strdup(info->installed_time);
    p->location = g_strdup(info->location);
    p->type = info->type;
    p->description = g_strdup(info->description);
    return p;
}

static inline void
lc_protocol_application_free_internal(LcProtocolApplication * p)
{
    g_free(p->package_name);
    g_free(p->app_name);
    g_free(p->version);
    g_free(p->installed_time);
    g_free(p->location);
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
        if (g_strcmp0(app->package_name, package) == 0) {
            return app;
        }
        list = g_list_next(list);
    }
    return NULL;
}

LcProtocolApplication *lc_protocol_get_application(const gchar * data)
{
    gchar **elements = g_strsplit(data, ":", -1);
    if (g_strv_length(elements) < LC_PROTOCOL_APPLICATION_SIZE) {
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
            if (app) {
                apps = g_list_append(apps, app);
            }
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

LcProtocolVersion *lc_protocol_version_new(const gchar * v)
{
    LcProtocolVersion *version =
        (LcProtocolVersion *) g_malloc(sizeof(LcProtocolVersion));
    version->version = g_strdup(v);
    return version;
}

void lc_protocol_version_free(LcProtocolVersion * v)
{
    if (v) {
        g_free(v->version);
        g_free(v);
    }
}

LcProtocolVersion *lc_protocol_create_version(const gchar * data)
{
    return lc_protocol_version_new(data);
}

LcProtocolVersion *lc_protocol_create_version_from_byte_array(GByteArray *
                                                              array)
{
    gchar *result = lc_util_get_string_from_byte_array(array, NULL);
    if (result == NULL || lc_protocol_get_result_from_string(result) !=
        LC_PROTOCOL_RESULT_OKAY) {
        g_warning("Failed to get lily version");
        return NULL;
    }
    LcProtocolVersion *version = lc_protocol_create_version(result + 4);
    g_free(result);
    return version;
}

LcProtocolPhone *lc_protocol_phone_new(const gchar * model,
                                       const gchar * brand,
                                       const gchar * number,
                                       const gchar * availableMemory,
                                       const gchar * totalMemory,
                                       const gchar * availabelSdCard,
                                       const gchar * totalSdCard,
                                       const gchar * availabelInternal,
                                       const gchar * totalInternal)
{
    LcProtocolPhone *phone =
        (LcProtocolPhone *) g_malloc(sizeof(LcProtocolPhone));
    phone->model = g_strdup(model);
    phone->brand = g_strdup(brand);
    phone->number = g_strdup(number);
    phone->availableMemory = g_strdup(availableMemory);
    phone->totalMemory = g_strdup(totalMemory);
    phone->availableSdCard = g_strdup(availabelSdCard);
    phone->totalSdCard = g_strdup(totalSdCard);
    phone->availabelInternal = g_strdup(availabelInternal);
    phone->totalInternal = g_strdup(totalInternal);
    return phone;
}

void lc_protocol_phone_free(LcProtocolPhone * phone)
{
    g_free(phone->model);
    g_free(phone->brand);
    g_free(phone->number);
    g_free(phone->availableMemory);
    g_free(phone->availableSdCard);
    g_free(phone->availabelInternal);
    g_free(phone->totalMemory);
    g_free(phone->totalSdCard);
    g_free(phone->totalInternal);

    g_free(phone);
}

LcProtocolPhone *lc_protocol_create_phone(const gchar * data)
{
    gchar **array = g_strsplit(data, "\n", -1);
    if (array == NULL || g_strv_length(array) <= LC_PROTOCOL_PHONE_SIZE) {
        g_strfreev(array);
        return NULL;
    }
    LcProtocolPhone *phone =
        lc_protocol_phone_new(array[0], array[1], array[2], array[3],
                              array[4], array[5], array[6], array[7],
                              array[8]);
    g_strfreev(array);
    return phone;
}

const gchar *lc_protocol_icon_command(const gchar * package)
{
    static gchar buf[256];
    g_snprintf(buf, sizeof(buf), LC_PROTOCOL_ICON, package);
    return buf;
}


LcProtocolSMS *lc_protocol_sms_new(LcProtocolSMSType type,
                                   const gchar * body,
                                   const gchar * address,
                                   const gchar * date, gint person,
                                   glong time)
{
    LcProtocolSMS *sms =
        (LcProtocolSMS *) g_slice_alloc0(sizeof(LcProtocolSMS));
    sms->type = type;
    sms->body = g_strdup(body);
    sms->address = g_strdup(address);
    sms->date = g_strdup(date);
    sms->person = person;
    sms->time = time;

    return sms;
}

LcProtocolSMS *lc_protocol_sms_new_take(LcProtocolSMSType type,
                                        gchar * body, gchar * address,
                                        gchar * date, gint person,
                                        glong time)
{
    LcProtocolSMS *sms =
        (LcProtocolSMS *) g_slice_alloc0(sizeof(LcProtocolSMS));
    sms->type = type;
    sms->body = body;
    sms->address = address;
    sms->date = date;
    sms->person = person;
    sms->time = time;
    return sms;
}

LcProtocolSMS *lc_protocol_sms_copy(LcProtocolSMS * self)
{
    LcProtocolSMS *sms =
        (LcProtocolSMS *) g_slice_alloc0(sizeof(LcProtocolSMS));
    sms->type = self->type;
    sms->body = g_strdup(self->body);
    sms->address = g_strdup(self->address);
    sms->date = g_strdup(self->date);
    sms->person = self->person;
    sms->time = self->time;

    return sms;
}

static void sms_hash_func(gpointer key, gpointer value, gpointer user_data)
{
    GList **list = ((GList **) user_data);
    *list = g_list_append(*list, value);
}

static gint sms_compare_func(gconstpointer a, gconstpointer b)
{
    LcProtocolSMS *sa = (LcProtocolSMS *) ((GList *) a)->data;
    LcProtocolSMS *sb = (LcProtocolSMS *) ((GList *) b)->data;

    if (sa->time > sb->time) {
        return -1;
    } else if (sa->time < sb->time) {
        return 1;
    }
    return 0;
}

GList *lc_protocol_create_sms_list(LcProtocolSMSType type,
                                   const gchar * data)
{
    GHashTable *table = g_hash_table_new(g_str_hash, g_str_equal);

    const gchar *cur = data;
    const gchar *colon;
    while ((colon = strchr(cur, ':'))) {
        gchar *date = g_strndup(cur, colon - cur);
        cur = colon + 1;
        if ((colon = strchr(cur, ':')) == NULL) {
            /* ERROR */
            g_free(date);
            break;
        }
        gchar *address = g_strndup(cur, colon - cur);
        cur = colon + 1;
        if ((colon = strchr(cur, ':')) == NULL) {
            /* ERROR */
            g_free(date);
            g_free(address);
            break;
        }
        gchar *person = g_strndup(cur, colon - cur);
        cur = colon + 1;
        gssize size = lc_util_size_from_hex(cur);
        if (size < 0) {
            /* ERROR */
            g_free(date);
            g_free(address);
            g_free(person);
            break;
        }
        cur = cur + 4;
        gchar *body = g_strndup(cur, size);
        cur = cur + size;
        LcProtocolSMS *sms =
            lc_protocol_sms_new_take(type, body, address, date,
                                     atoi(person), atol(date));
        g_free(person);
        GList *lp =
            (GList *) g_hash_table_lookup(table, (gconstpointer) address);
        lp = g_list_append(lp, sms);
        g_hash_table_replace(table, (gpointer) address, (gpointer) lp);
    }

    GList *ret = NULL;
    g_hash_table_foreach(table, sms_hash_func, &ret);
    g_hash_table_destroy(table);

    ret = g_list_sort(ret, (GCompareFunc) sms_compare_func);

    return ret;
}

void lc_protocol_free_sms_list(GList * list)
{
    GList *lp = list;
    while (lp) {
        GList *l = (GList *) lp->data;
        g_list_free_full(l, (GDestroyNotify) lc_protocol_sms_free);
        lp = g_list_next(lp);
    }
    g_list_free(list);
}

void lc_protocol_sms_free(LcProtocolSMS * sms)
{
    g_free(sms->body);
    g_free(sms->address);
    g_free(sms->date);

    g_slice_free1(sizeof(LcProtocolSMS), sms);
}
