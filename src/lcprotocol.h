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

/*
 * the communication protocol between LUCY and LILY
 */

#ifndef __LC_PROTOCOL_H__
#define __LC_PROTOCOL_H__
#include <glib-2.0/glib.h>
#include <json-glib/json-glib.h>

/* all commands */
#define LC_PROTOCOL_APPLICATIONS "application"
#define LC_PROTOCOL_VERSION      "version"
#define LC_PROTOCOL_PHONE        "phone"
#define LC_PROTOCOL_ICON         "icon:%s"
#define LC_PROTOCOL_SMS  		   "sms"
#define LC_PROTOCOL_CONTACT		 "contact"

#define LC_PROTOCOL_HDR_LEN     (4)

#define LC_PROTOCOL_RETCODE_OKAY	10086

typedef enum {
    LC_PROTOCOL_RESULT_OKAY,
    LC_PROTOCOL_RESULT_FAIL,
} LcProtocolResult;

LcProtocolResult lc_protocol_get_result_from_string(const gchar * str);
LcProtocolResult lc_protocol_get_result_from_bytes(GBytes * bytes);


/******************************APPLICATIONS******************************/
typedef enum {
    LC_PROTOCOL_APPLICATION_TYPE_ALL,
    LC_PROTOCOL_APPLICATION_TYPE_SYSTEM,
    LC_PROTOCOL_APPLICATION_TYPE_THIRD,
    LC_PROTOCOL_APPLICATION_TYPE_UNKNOWN,
} LcProtocolApplicationType;

typedef struct {
    gchar *package_name;
    gchar *app_name;
    gchar *version;
    gchar *installed_time;
    gchar *location;
    gchar *description;
    LcProtocolApplicationType type;
} LcProtocolApplication;

LcProtocolApplication *lc_protocol_application_new(const gchar *
                                                   packageName,
                                                   const gchar * appName,
                                                   const gchar * version,
                                                   const gchar *
                                                   installedTime,
                                                   const gchar *
                                                   installedLocation,
                                                   const gchar * type,
                                                   const gchar *
                                                   description);
void lc_protocol_application_free(LcProtocolApplication * p);
LcProtocolApplication *lc_protocol_application_copy(const
                                                    LcProtocolApplication *
                                                    info);

/*
 * 从lily返回的json数据中解析出应用列表
 */
LcProtocolApplication
    * lc_protoocl_application_create_from_json_object(JsonObject * obj);
GList *lc_protocol_application_list_create_from_json_array(JsonArray *
                                                           array);
/*
 * free a list of LcProtocolApplication
 */
void lc_protocol_free_application_list(GList * list);

LcProtocolApplication *lc_protocol_application_find(GList * list,
                                                    const gchar * package);


/*******************************PHONE************************************/

typedef struct {
    gchar *model;
    gchar *brand;
    gchar *number;              /* phone number */
    gint64 avail_mem;
    gint64 total_mem;
    gint64 avail_external;
    gint64 total_external;
    gint64 avail_data;
    gint64 total_data;
} LcProtocolPhone;

LcProtocolPhone *lc_protocol_phone_new(const gchar * model,
                                       const gchar * brand,
                                       const gchar * number,
                                       gint64 avail_mem,
                                       gint64 total_mem,
                                       gint64 avail_external,
                                       gint64 total_external,
                                       gint64 avail_data,
                                       gint64 total_data);

void lc_protocol_phone_free(LcProtocolPhone * phone);

LcProtocolPhone *lc_protocol_phone_create_from_json_object(JsonObject *
                                                           obj);


/**********************************ICON*********************************/

const gchar *lc_protocol_icon_command(const gchar * package);


/**********************************SMS*********************************/
typedef enum {
    LC_PROTOCOL_SMS_TYPE_INBOX = 1,
    LC_PROTOCOL_SMS_TYPE_SENT,
} LcProtocolSMSType;

typedef struct {
    gint thread_id;
    LcProtocolSMSType type;
    gchar *body;                /* the body of SMS */
    gchar *address;             /* the author of SMS (phone number) */
    guint64 date;               /* date in second */
    gint person;                /* person in contacts */
} LcProtocolSMS;

LcProtocolSMS *lc_protocol_sms_new(gint thread_id,
                                   LcProtocolSMSType type,
                                   const gchar * body,
                                   const gchar * address,
                                   guint64 date, gint person);
void lc_protocol_sms_free(LcProtocolSMS * sms);

LcProtocolSMS *lc_protocol_sms_new_take(gint thread_id,
                                        LcProtocolSMSType type,
                                        gchar * body, gchar * address,
                                        guint64 date, gint person);
LcProtocolSMS *lc_protocol_sms_copy(LcProtocolSMS * self);

LcProtocolSMS *lc_protocol_sms_create_from_json_object(JsonObject * obj);
GList *lc_protocol_sms_list_create_from_json_array(JsonArray * array);
void lc_protocol_free_sms_list(GList * list);

/**********************************CONTACT*******************************/
typedef struct {
    gint64 id;
    gchar *name;
    gchar *number;
} LcProtocolContact;

LcProtocolContact *lc_protocol_contact_new(gint64 id, const gchar * name,
                                           const gchar * number);
void lc_protocol_contact_free(LcProtocolContact * contact);

LcProtocolContact *lc_protocol_contact_create_from_json_object(JsonObject *
                                                               obj);
GList *lc_protocol_contact_list_create_from_json_array(JsonArray * array);

void lc_protocol_contact_list_free(GList * list);

#endif
