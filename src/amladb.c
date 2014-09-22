/*
 * amladb.c
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

#include "amladb.h"
#include "libadb/sysdeps.h"
#include "libadb/adb_client.h"

/*
 * install_app在commandline.c中定义，但没有在头文件中声明过
 */
int install_app(transport_type transport, char *serial, int argc,
                char **argv);

typedef struct {
    gchar *buf;
} AmlTaskData;

static AmlTaskData *aml_task_data_new(const gchar * buf)
{
    AmlTaskData *data = (AmlTaskData *) g_malloc(sizeof(AmlTaskData));
    data->buf = g_strdup(buf);
    return data;
}

static void aml_task_data_free(AmlTaskData * data)
{
    g_free(data->buf);
    g_free(data);
}

/***********************aml_adb_command****************************/

static void aml_adb_command_thread(GTask * task,
                                   gpointer source_object,
                                   gpointer task_data,
                                   GCancellable * cancellable)
{
    AmlTaskData *data = (AmlTaskData *) task_data;
    int ret = adb_command(data->buf);
    g_task_return_int(task, ret);
}

void aml_adb_command(const gchar * buf, GAsyncReadyCallback callback,
                     gpointer data)
{
    GTask *task = g_task_new(NULL, NULL, callback, data);
    AmlTaskData *task_data = aml_task_data_new(buf);
    g_task_set_task_data(task, task_data,
                         (GDestroyNotify) aml_task_data_free);
    g_task_run_in_thread(task, aml_adb_command_thread);
}

int aml_adb_command_finish(GAsyncResult * result)
{
    return g_task_propagate_int(G_TASK(result), NULL);
}

/**********************aml_adb_connect***************************/

static void aml_adb_connect_thread(GTask * task, gpointer source_object,
                                   gpointer task_data,
                                   GCancellable * cancellable)
{
    AmlTaskData *data = (AmlTaskData *) task_data;
    int ret = adb_connect(data->buf);
    g_task_return_int(task, ret);
}

void aml_adb_connect(const gchar * buf, GAsyncReadyCallback callback,
                     gpointer data)
{
    GTask *task = g_task_new(NULL, NULL, callback, data);
    AmlTaskData *task_data = aml_task_data_new(buf);
    g_task_set_task_data(task, task_data,
                         (GDestroyNotify) aml_task_data_free);
    g_task_run_in_thread(task, aml_adb_connect_thread);
}

int aml_adb_connect_finish(GAsyncResult * result)
{
    return g_task_propagate_int(G_TASK(result), NULL);
}

/**************************************************************/

void aml_adb_start_server(GAsyncReadyCallback callback, gpointer data)
{
    aml_adb_connect("host:start-server", callback, data);
}


void aml_adb_forward(guint local, guint remote,
                     GAsyncReadyCallback callback, gpointer data)
{
    gchar buf[64];
    g_snprintf(buf, sizeof(buf), "host-usb:forward:tcp:%u;tcp:%u", local,
               remote);
    aml_adb_command(buf, callback, data);
}


static void aml_adb_am_start_thread(GTask * task, gpointer source_object,
                                    gpointer task_data,
                                    GCancellable * cancellable)
{
    AmlTaskData *data = (AmlTaskData *) task_data;
    int fd = adb_connect(data->buf);
    if (fd < 0) {
        g_task_return_int(task, fd);
    }
    gchar buf[1024];
    int n = adb_read(fd, buf, sizeof(buf));
    while ((n = adb_read(fd, buf, sizeof(buf))) > 0) {
        if (g_strstr_len(buf, n, "Error")) {
            adb_close(fd);
            g_task_return_int(task, -1);
        }
    }
    adb_close(fd);
    g_task_return_int(task, n);
}

void aml_adb_am_start(const gchar * activity,
                      GAsyncReadyCallback callback, gpointer data)
{
    gchar buf[64];
    g_snprintf(buf, sizeof(buf), "shell:am start -n %s", activity);
    GTask *task = g_task_new(NULL, NULL, callback, data);
    AmlTaskData *task_data = aml_task_data_new(buf);
    g_task_set_task_data(task, task_data,
                         (GDestroyNotify) aml_task_data_free);
    g_task_run_in_thread(task, aml_adb_am_start_thread);
}

static void aml_adb_install_app_thread(GTask * task,
                                       gpointer source_object,
                                       gpointer task_data,
                                       GCancellable * cancellable)
{
    gchar *argv[2] = { "install", NULL };
    AmlTaskData *data = (AmlTaskData *) task_data;
    argv[1] = data->buf;
    int ret = install_app(kTransportAny, NULL, 2, argv);

    g_task_return_int(task, ret);
}

/*
 * 安装apk到android手机
 */
void aml_adb_install_app(const gchar * filepath,
                         GAsyncReadyCallback callback, gpointer data)
{
    GTask *task = g_task_new(NULL, NULL, callback, data);
    AmlTaskData *task_data = aml_task_data_new(filepath);
    g_task_set_task_data(task, task_data,
                         (GDestroyNotify) aml_task_data_free);
    g_task_run_in_thread(task, aml_adb_install_app_thread);
}

int aml_adb_install_app_finish(GAsyncResult * result)
{
    return g_task_propagate_int(G_TASK(result), NULL);
}
