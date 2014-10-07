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
#include "lcadb.h"

#define LILY_ACTIVITY_NAME   "org.wl.ll/.MainActivity"
#define LILY_APK_NAME    "apk/app-debug.apk"
#define LILY_VERSION    "1"

#define HOST_IP_ADDRESSS    "127.0.0.1"

static GList *sockets = NULL;

typedef struct {
    gchar *buf;
    gpointer callback;
    gpointer user_data;
} LcCommanderData;

static inline LcCommanderData *lc_commander_data_new(const gchar * buf,
                                                     gpointer callback,
                                                     gpointer user_data)
{
    LcCommanderData *data =
        (LcCommanderData *) g_malloc(sizeof(LcCommanderData));
    data->buf = g_strdup(buf);
    data->callback = callback;
    data->user_data = user_data;
    return data;
}

static void lc_commander_data_free(LcCommanderData * data)
{
    g_free(data->buf);
    g_free(data);
}

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

static void on_command(GObject * source_object, GAsyncResult * result,
                       gpointer user_data)
{
    LcCommanderData *data = (LcCommanderData *) user_data;
    GByteArray *res = lc_socket_send_command_async_finish(result);
    ((LcCommanderCommandCallback) data->callback) (data->buf, res,
                                                   data->user_data);
    lc_commander_data_free(data);
}

static void on_socket_connect(GObject * source_object,
                              GAsyncResult * result, gpointer user_data)
{
    LcCommanderData *data = (LcCommanderData *) user_data;
    gboolean ret = lc_socket_connect_async_finish(result);
    if (ret) {
        g_message("New Socket Connection Established!");
        lc_socket_send_command_async(LC_SOCKET(source_object), data->buf,
                                     on_command, data);
        sockets = g_list_append(sockets, source_object);
    } else {
        g_warning("New Socket Connection not Established!!!");
        ((LcCommanderCommandCallback) data->callback) (data->buf, NULL,
                                                       data->user_data);
        lc_commander_data_free(data);
    }
}

void lc_commander_send_command_async(const gchar * cmd,
                                     LcCommanderCommandCallback callback,
                                     gpointer user_data)
{
    LcCommanderData *cdata =
        lc_commander_data_new(cmd, callback, user_data);
    LcSocket *s = lc_commander_find_idle_socket();
    if (s) {
        g_message("Using Old Socket Sonnection. Sending Command: %s", cmd);
        lc_socket_send_command_async(s, cmd, on_command, cdata);
    } else {
        g_message("No Idle Socket is found,creating a new one!");
        LcSocket *socket =
            lc_socket_new(HOST_IP_ADDRESSS, ADB_FORWARD_LOCAL);
        lc_socket_connect_async(socket, on_socket_connect, cdata);
    }
}

static void on_command_version(const gchar * cmd, GByteArray * array,
                               gpointer user_data)
{
    LcCommanderData *cdata = (LcCommanderData *) user_data;
    gchar *result = lc_util_get_string_from_byte_array(array, NULL);
    LcCommanderInitResult ret = LC_COMMANDER_INIT_OK;
    if (result == NULL
        || lc_protocol_get_result_from_string(result) !=
        LC_PROTOCOL_RESULT_OKAY) {
        g_warning("Failed to get lily version");
        ret = LC_COMMANDER_INIT_FAILED_VERSION;
    } else {
        LcProtocolVersion *version =
            lc_protocol_create_version(result + LC_PROTOCOL_HDR_LEN);
        if (g_strcmp0(LILY_VERSION, version->version)) {
            /* Lily version not match */
            g_warning("Lily version doesn't match!!");
            ret = LC_COMMANDER_INIT_FAILED_VERSION;
        }
        g_message("Lily Version: %s", result + LC_PROTOCOL_HDR_LEN);
        lc_protocol_version_free(version);
    }
    ((LcCommanderInitCallback) cdata->callback) (ret, cdata->user_data);
    lc_commander_data_free(cdata);
    g_free(result);
}

static gboolean on_request_version_timeout(gpointer user_data)
{
    lc_commander_send_command_async(LC_PROTOCOL_VERSION,
                                    on_command_version, user_data);
    return FALSE;
}

static void lc_commander_request_version(gpointer user_data)
{
    g_timeout_add(1000, on_request_version_timeout, user_data);
}

/****************************Initialize Connection***************************/
static void on_lily_activity_start_final(GObject * source_object,
                                         GAsyncResult * res,
                                         gpointer user_data)
{
    LcCommanderData *cdata = (LcCommanderData *) user_data;
    if (lc_adb_am_start_finish(res)) {
        ((LcCommanderInitCallback)
         cdata->callback) (LC_COMMANDER_INIT_FAILED_START,
                           cdata->user_data);
        lc_commander_data_free(cdata);
    } else {
        lc_commander_request_version(cdata);
    }
}

static void on_install_lily(GObject * source_object,
                            GAsyncResult * res, gpointer user_data)
{
    int ret = lc_adb_install_app_finish(res);
    if (ret) {
        LcCommanderData *cdata = (LcCommanderData *) user_data;
        ((LcCommanderInitCallback)
         cdata->callback) (LC_COMMANDER_INIT_FAILED_INSTALL,
                           cdata->user_data);
        lc_commander_data_free(cdata);
        return;
    }
    lc_adb_am_start(LILY_ACTIVITY_NAME, on_lily_activity_start_final,
                    user_data);
}

static void on_lily_activity_start(GObject * source_object,
                                   GAsyncResult * res, gpointer user_data)
{
    LcCommanderData *cdata = (LcCommanderData *) user_data;
    if (lc_adb_am_start_finish(res)) {
        const gchar *apkpath = lc_util_get_resource_by_name(LILY_APK_NAME);
        if (apkpath == NULL) {
            ((LcCommanderInitCallback)
             cdata->callback) (LC_COMMANDER_INIT_FAILED_INSTALL,
                               cdata->user_data);
            lc_commander_data_free(cdata);
        } else {
            lc_adb_install_app(apkpath, on_install_lily, user_data);
        }
    } else {
        lc_commander_request_version(cdata);
    }
}

static void on_tcp_port_forward(GObject * source_object,
                                GAsyncResult * res, gpointer user_data)
{
    int ret = lc_adb_forward_finish(res);
    if (ret == 0) {
        lc_adb_am_start(LILY_ACTIVITY_NAME, on_lily_activity_start,
                        user_data);
    } else {
        g_warning("Failed to set TCP port forward");
        LcCommanderData *cdata = (LcCommanderData *) user_data;
        ((LcCommanderInitCallback)
         cdata->callback) (LC_COMMANDER_INIT_FAILED_FORWARD,
                           cdata->user_data);
        lc_commander_data_free(cdata);
    }
}

static void on_start_server(GObject * source_object,
                            GAsyncResult * res, gpointer data)
{
    int ret = lc_adb_start_server_finish(res);
    if (ret == 0) {
        lc_adb_forward(ADB_FORWARD_LOCAL, ADB_FORWARD_REMOTE,
                       on_tcp_port_forward, data);
    } else {
        /* ERROR, failed to start(or connect to) adb server */
        LcCommanderData *cdata = (LcCommanderData *) data;
        ((LcCommanderInitCallback)
         cdata->callback) (LC_COMMANDER_INIT_FAILED_SERVER,
                           cdata->user_data);
        lc_commander_data_free(cdata);
    }
}

void lc_commander_init_async(LcCommanderInitCallback callback,
                             gpointer data)
{
    LcCommanderData *cdata = lc_commander_data_new(NULL, callback, data);
    lc_adb_start_server(on_start_server, cdata);
}
