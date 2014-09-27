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

static void onSocketConnection(GObject * source_object,
                               GAsyncResult * res, gpointer user_data)
{
    LcCommanderData *data = (LcCommanderData *) user_data;
    if (lc_socket_connect_async_finish(res)) {
        g_message("New Connection is Established!");
        lc_socket_send_command_async(LC_SOCKET(source_object),
                                     data->buf, data->callback,
                                     data->user_data);
        sockets = g_list_append(sockets, source_object);
        g_message("Sending Command: %s", data->buf);
    } else {
        ((GAsyncReadyCallback) data->callback) (NULL, NULL,
                                                data->user_data);
        g_message("Connection is not Established!");
    }
    g_object_unref(source_object);
    lc_commander_data_free(data);
}

void lc_commander_send_command(const gchar * cmd,
                               GAsyncReadyCallback callback,
                               gpointer user_data)
{
    LcSocket *s = lc_commander_find_idle_socket();
    if (s) {
        lc_socket_send_command_async(s, cmd, callback, user_data);
        return;
    }
    g_message("No Idle Socket is found,creating a new one!");
    LcSocket *socket = lc_socket_new("127.0.0.1", ADB_FORWARD_LOCAL);
    LcCommanderData *data =
        lc_commander_data_new(cmd, callback, user_data);
    lc_socket_connect_async(socket, onSocketConnection, data);
}

GByteArray *lc_commander_send_command_finish(GAsyncResult * res)
{
    if (res == NULL) {
        return NULL;
    }
    return lc_socket_send_command_async_finish(res);
}

/****************************Initialize Connection***************************/
static void onActivityStartFinal(GObject * source_object,
                                 GAsyncResult * res, gpointer user_data)
{
    LcCommanderData *cdata = (LcCommanderData *) user_data;
    if (lc_adb_am_start_finish(res)) {
        ((LcCommanderInitCallback) cdata->
         callback) (LC_COMMANDER_INIT_FAILED_START, cdata->user_data);
    } else {
        ((LcCommanderInitCallback) cdata->callback) (LC_COMMANDER_INIT_OK,
                                                     cdata->user_data);
    }
    lc_commander_data_free(cdata);
}

static void onInstallLily(GObject * source_object,
                          GAsyncResult * res, gpointer user_data)
{
    int ret = lc_adb_install_app_finish(res);
    if (ret) {
        LcCommanderData *cdata = (LcCommanderData *) user_data;
        ((LcCommanderInitCallback) cdata->
         callback) (LC_COMMANDER_INIT_FAILED_INSTALL, cdata->user_data);
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
            ((LcCommanderInitCallback) cdata->
             callback) (LC_COMMANDER_INIT_FAILED_INSTALL,
                        cdata->user_data);
            lc_commander_data_free(cdata);
        } else {
            lc_adb_install_app(apkpath, onInstallLily, user_data);
        }
    } else {
        ((LcCommanderInitCallback) cdata->callback) (LC_COMMANDER_INIT_OK,
                                                     cdata->user_data);
        lc_commander_data_free(cdata);
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
        ((LcCommanderInitCallback) cdata->
         callback) (LC_COMMANDER_INIT_FAILED_FORWARD, cdata->user_data);
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
        ((LcCommanderInitCallback) cdata->
         callback) (LC_COMMANDER_INIT_FAILED_SERVER, cdata->user_data);
        lc_commander_data_free(cdata);
    }
}

void lc_commander_init(LcCommanderInitCallback callback, gpointer data)
{
    LcCommanderData *cdata = lc_commander_data_new(NULL, callback, data);
    lc_adb_start_server(onStartServer, cdata);
}
