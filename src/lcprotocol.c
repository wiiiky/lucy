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

LcProtocolApplication
    * lc_protoocl_application_create_from_json_object(JsonObject * obj)
{
    const gchar *package_name =
        json_object_get_string_member(obj, "package_name");
    const gchar *app_name = json_object_get_string_member(obj, "app_name");
    const gchar *version_name =
        json_object_get_string_member(obj, "version_name");
    const gchar *location = json_object_get_string_member(obj, "location");
    const gchar *install_time =
        json_object_get_string_member(obj, "install_time");
    const gchar *type = json_object_get_string_member(obj, "type");
//  gint64 size=json_object_get_int_member(obj,"size");
    const gchar *description =
        json_object_get_string_member(obj, "description");

    LcProtocolApplication *app =
        lc_protocol_application_new(package_name, app_name,
                                    version_name,
                                    install_time,
                                    location,
                                    type,
                                    description);
    return app;
}

GList *lc_protocol_application_list_create_from_json_array(JsonArray *
                                                           array)
{
    GList *elements = json_array_get_elements(array);
    GList *apps = NULL;
    GList *lp = elements;
    while (lp) {
        JsonNode *node = (JsonNode *) lp->data;
        JsonObject *obj = json_node_get_object(node);
        LcProtocolApplication *app =
            lc_protoocl_application_create_from_json_object(obj);
        apps = g_list_append(apps, app);
        lp = g_list_next(lp);
    }
    g_list_free(elements);
    return apps;
}

void lc_protocol_free_application_list(GList * list)
{
    g_list_free_full(list, (GDestroyNotify) lc_protocol_application_free);
}


LcProtocolPhone *lc_protocol_phone_new(const gchar * model,
                                       const gchar * brand,
                                       const gchar * number,
                                       gint64 avail_mem,
                                       gint64 total_mem,
                                       gint64 avail_external,
                                       gint64 total_external,
                                       gint64 avail_data,
                                       gint64 total_data)
{
    LcProtocolPhone *phone =
        (LcProtocolPhone *) g_malloc(sizeof(LcProtocolPhone));
    phone->model = g_strdup(model);
    phone->brand = g_strdup(brand);
    phone->number = g_strdup(number);
    phone->avail_mem = avail_mem;
    phone->total_mem = total_mem;
    phone->avail_external = avail_external;
    phone->total_external = total_external;
    phone->avail_data = avail_data;
    phone->total_data = total_data;
    return phone;
}

LcProtocolPhone *lc_protocol_phone_create_from_json_object(JsonObject *
                                                           obj)
{
    const gchar *model = json_object_get_string_member(obj, "model");
    const gchar *brand = json_object_get_string_member(obj, "brand");
    const gchar *number = json_object_get_string_member(obj, "number");
    gint64 avail_mem = json_object_get_int_member(obj, "avail_mem");
    gint64 total_mem = json_object_get_int_member(obj, "total_mem");
    gint64 avail_external =
        json_object_get_int_member(obj, "avail_external");
    gint64 total_external =
        json_object_get_int_member(obj, "total_external");
    gint64 avail_data = json_object_get_int_member(obj, "avail_data");
    gint64 total_data = json_object_get_int_member(obj, "total_data");
    LcProtocolPhone *phone = lc_protocol_phone_new(model, brand, number,
                                                   avail_mem, total_mem,
                                                   avail_external,
                                                   total_external,
                                                   avail_data,
                                                   total_data);
    return phone;
}

void lc_protocol_phone_free(LcProtocolPhone * phone)
{
    g_free(phone->model);
    g_free(phone->brand);
    g_free(phone->number);

    g_free(phone);
}

const gchar *lc_protocol_icon_command(const gchar * package)
{
    static gchar buf[256];
    g_snprintf(buf, sizeof(buf), LC_PROTOCOL_ICON, package);
    return buf;
}


LcProtocolSMS *lc_protocol_sms_new(gint thread_id,
                                   LcProtocolSMSType type,
                                   const gchar * body,
                                   const gchar * address,
                                   guint64 date, gint person)
{
    LcProtocolSMS *sms =
        (LcProtocolSMS *) g_slice_alloc0(sizeof(LcProtocolSMS));
    sms->thread_id = thread_id;
    sms->type = type;
    sms->body = g_strdup(body);
    sms->address = g_strdup(address);
    sms->date = date;
    sms->person = person;

    return sms;
}

LcProtocolSMS *lc_protocol_sms_new_take(gint thread_id,
                                        LcProtocolSMSType type,
                                        gchar * body, gchar * address,
                                        guint64 date, gint person)
{
    LcProtocolSMS *sms =
        (LcProtocolSMS *) g_slice_alloc0(sizeof(LcProtocolSMS));
    sms->thread_id = thread_id;
    sms->type = type;
    sms->body = body;
    sms->address = address;
    sms->date = date;
    sms->person = person;
    return sms;
}

LcProtocolSMS *lc_protocol_sms_copy(LcProtocolSMS * self)
{
    LcProtocolSMS *sms =
        (LcProtocolSMS *) g_slice_alloc0(sizeof(LcProtocolSMS));
    sms->thread_id = self->thread_id;
    sms->type = self->type;
    sms->body = g_strdup(self->body);
    sms->address = g_strdup(self->address);
    sms->date = self->date;
    sms->person = self->person;

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

    if (sa->date > sb->date) {
        return -1;
    } else if (sa->date < sb->date) {
        return 1;
    }
    return 0;
}


LcProtocolSMS *lc_protocol_sms_create_from_json_object(JsonObject * obj)
{
    gint64 thread_id = json_object_get_int_member(obj, "thread_id");
    gint64 type = json_object_get_int_member(obj, "type");
    gint64 date = json_object_get_int_member(obj, "date");
    const gchar *address = json_object_get_string_member(obj, "address");
    gint64 person = json_object_get_int_member(obj, "person");
    const gchar *body = json_object_get_string_member(obj, "body");

    LcProtocolSMS *sms =
        lc_protocol_sms_new((gint) thread_id, (LcProtocolSMSType) type,
                            body, address, date, (gint) person);
    return sms;
}

GList *lc_protocol_sms_list_create_from_json_array(JsonArray * array)
{
    GHashTable *table = g_hash_table_new(g_direct_hash, g_direct_equal);
    GList *nodes = json_array_get_elements(array);
    GList *lp = nodes;
    while (lp) {
        JsonNode *node = (JsonNode *) lp->data;
        JsonObject *obj = json_node_get_object(node);
        LcProtocolSMS *sms = lc_protocol_sms_create_from_json_object(obj);

        GList *l = (GList *) g_hash_table_lookup(table,
                                                 GINT_TO_POINTER
                                                 (sms->thread_id));
        l = g_list_append(l, sms);
        g_hash_table_replace(table, GINT_TO_POINTER(sms->thread_id),
                             (gpointer) l);

        lp = g_list_next(lp);
    }
    g_list_free(nodes);

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

    g_slice_free1(sizeof(LcProtocolSMS), sms);
}

LcProtocolContact *lc_protocol_contact_new(gint64 id, const gchar * name,
                                           const gchar * number)
{
    LcProtocolContact *contact =
        (LcProtocolContact *) g_slice_alloc0(sizeof(LcProtocolContact));
    contact->id = id;
    contact->name = g_strdup(name);
    contact->number = g_strdup(number);

    return contact;
}

void lc_protocol_contact_free(LcProtocolContact * contact)
{
    g_free(contact->name);
    g_free(contact->number);

    g_slice_free1(sizeof(LcProtocolContact), contact);
}

LcProtocolContact *lc_protocol_contact_create_from_json_object(JsonObject *
                                                               obj)
{
    gint64 id = json_object_get_int_member(obj, "id");
    const gchar *name = json_object_get_string_member(obj, "name");
    const gchar *number = json_object_get_string_member(obj, "number");
    return lc_protocol_contact_new(id, name, number);
}

GList *lc_protocol_contact_list_create_from_json_array(JsonArray * array)
{
    GList *elements = json_array_get_elements(array);
    GList *list = NULL;
    GList *lp = elements;
    while (lp) {
        JsonNode *node = (JsonNode *) lp->data;
        JsonObject *obj = json_node_get_object(node);
        LcProtocolContact *contact =
            lc_protocol_contact_create_from_json_object(obj);
        list = g_list_append(list, contact);
        lp = g_list_next(lp);
    }
    g_list_free(elements);
    return list;
}

void lc_protocol_contact_list_free(GList * list)
{
    g_list_free_full(list, (GDestroyNotify) lc_protocol_contact_free);
}
