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
    guint n;
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

void lc_commander_send_command_async(const gchar * cmd,
                                     LcCommanderCommandCallback callback,
                                     gpointer user_data)
{
    lc_commander_send_command_async_n(cmd, callback, user_data, 1);
}

static void lc_commander_send_command_thread(GTask * task,
                                             gpointer source_object,
                                             gpointer task_data,
                                             GCancellable * cancellable)
{
    LcCommanderData *cdata = (LcCommanderData *) task_data;
    const gchar *cmd = cdata->buf;
    guint n = cdata->n;
    GByteArray *array = lc_commander_send_command(cmd, n);
    g_task_return_pointer(task, array,
                          (GDestroyNotify) g_byte_array_unref);
}

static void onCommandSend(GObject * source_object, GAsyncResult * result,
                          gpointer user_data)
{
    LcCommanderData *data =
        (LcCommanderData *) g_task_get_task_data(G_TASK(result));
    GByteArray *res =
        (GByteArray *) g_task_propagate_pointer(G_TASK(result), NULL);
    ((LcCommanderCommandCallback) data->callback) (res, data->user_data);
}

void lc_commander_send_command_async_n(const gchar * cmd,
                                       LcCommanderCommandCallback callback,
                                       gpointer data, guint n)
{
    LcCommanderData *cdata = lc_commander_data_new(cmd, callback, data);
    cdata->n = n;
    GTask *task = g_task_new(NULL, NULL, onCommandSend, NULL);
    g_task_set_task_data(task, cdata,
                         (GDestroyNotify) lc_commander_data_free);
    g_task_run_in_thread(task, lc_commander_send_command_thread);
}

GByteArray *lc_commander_send_command(const gchar * cmd, guint n)
{
    int i;
    for (i = 1; i <= n; i++) {
        LcSocket *s = lc_commander_find_idle_socket();
        if (s) {
            g_message("Using Old Socket Sonnection. Sending Command: %s",
                      cmd);
            GByteArray *array = lc_socket_send_command(s, cmd);
            if (array != NULL) {
                return array;
            }
        } else {
            g_message("No Idle Socket is found,creating a new one!");
            LcSocket *socket =
                lc_socket_new(HOST_IP_ADDRESSS, ADB_FORWARD_LOCAL);
            gboolean ret = lc_socket_connect(socket);
            if (ret == TRUE) {
                GByteArray *array = lc_socket_send_command(socket, cmd);
                if (array != NULL) {
                    sockets = g_list_append(sockets, socket);
                    return array;
                }
            }
        }
        sleep(i);
    }
    return NULL;
}

static void onVersionResponse(GByteArray * array, gpointer user_data)
{
    LcCommanderData *cdata = (LcCommanderData *) user_data;
    gchar *result = lc_util_get_string_from_byte_array(array, NULL);
    LcCommanderInitResult ret = LC_COMMANDER_INIT_OK;
    if (result == NULL || lc_protocol_get_result_from_string(result) !=
        LC_PROTOCOL_RESULT_OKAY) {
        g_warning("Failed to get lily version");
        ret = LC_COMMANDER_INIT_FAILED_VERSION;
    }
    LcProtocolVersion *version = lc_protocol_create_version(result + 4);
    if (g_strcmp0(LILY_VERSION, version->version)) {
        /* Lily version not match */
        g_warning("Lily version doesn't match!!");
        ret = LC_COMMANDER_INIT_FAILED_VERSION;
    }
    g_message("Lily Version: %s", result + 4);
    ((LcCommanderInitCallback) cdata->callback) (ret, cdata->user_data);
    lc_commander_data_free(cdata);
    g_free(result);
    lc_protocol_version_free(version);
}

/****************************Initialize Connection***************************/
static void onActivityStartFinal(GObject * source_object,
                                 GAsyncResult * res, gpointer user_data)
{
    LcCommanderData *cdata = (LcCommanderData *) user_data;
    if (lc_adb_am_start_finish(res)) {
        ((LcCommanderInitCallback)
         cdata->callback) (LC_COMMANDER_INIT_FAILED_START,
                           cdata->user_data);
        lc_commander_data_free(cdata);
    } else {
        lc_commander_send_command_async_n(LC_COMMAND_VERSION,
                                          onVersionResponse, cdata, 3);
    }
}

static void onInstallLily(GObject * source_object,
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
    lc_adb_am_start(LILY_ACTIVITY_NAME, onActivityStartFinal, user_data);
}

static void onActivityStart(GObject * source_object,
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
            lc_adb_install_app(apkpath, onInstallLily, user_data);
        }
    } else {
        lc_commander_send_command_async_n(LC_COMMAND_VERSION,
                                          onVersionResponse, cdata, 3);
    }
}

static void onForward(GObject * source_object,
                      GAsyncResult * res, gpointer user_data)
{
    int ret = lc_adb_forward_finish(res);
    if (ret == 0) {
        lc_adb_am_start(LILY_ACTIVITY_NAME, onActivityStart, user_data);
    } else {
        LcCommanderData *cdata = (LcCommanderData *) user_data;
        ((LcCommanderInitCallback)
         cdata->callback) (LC_COMMANDER_INIT_FAILED_FORWARD,
                           cdata->user_data);
        lc_commander_data_free(cdata);
    }
}

static void onStartServer(GObject * source_object,
                          GAsyncResult * res, gpointer data)
{
    int ret = lc_adb_start_server_finish(res);
    if (ret == 0) {
        lc_adb_forward(ADB_FORWARD_LOCAL, ADB_FORWARD_REMOTE,
                       onForward, data);
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
    lc_adb_start_server(onStartServer, cdata);
}
