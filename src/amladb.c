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

typedef struct {
    gchar *buf;
} CommandData;

static CommandData *command_data_new(const gchar * buf)
{
    CommandData *data = (CommandData *) g_malloc(sizeof(CommandData));
    data->buf = g_strdup(buf);
    return data;
}

static void command_data_free(CommandData * data)
{
    g_free(data->buf);
    g_free(data);
}

static void aml_adb_command_thread(GTask * task,
                                   gpointer source_object,
                                   gpointer task_data,
                                   GCancellable * cancellable)
{
    CommandData *data = (CommandData *) task_data;
    int ret = adb_command(data->buf);
    g_task_return_int(task, ret);
}

void aml_adb_command(const gchar * buf, GAsyncReadyCallback callback,
                     gpointer data)
{
    GTask *task = g_task_new(NULL, NULL, callback, data);
    CommandData *task_data = command_data_new(buf);
    g_task_set_task_data(task, task_data,
                         (GDestroyNotify) command_data_free);
    g_task_run_in_thread(task, aml_adb_command_thread);
}

int aml_adb_command_finish(GAsyncResult * result)
{
    return g_task_propagate_int(G_TASK(result), NULL);
}


static void aml_adb_start_server_thread(GTask * task,
                                        gpointer source_object,
                                        gpointer task_data,
                                        GCancellable * cancellable)
{
    int ret = adb_connect("host:start-server");
    g_task_return_int(task, ret);
}

void aml_adb_start_server(GAsyncReadyCallback callback, gpointer data)
{
    GTask *task = g_task_new(NULL, NULL, callback, data);
    g_task_run_in_thread(task, aml_adb_start_server_thread);
}

int aml_adb_start_server_finish(GAsyncResult * result)
{
    return g_task_propagate_int(G_TASK(result), NULL);
}


void aml_adb_forward(guint local, guint remote,
                     GAsyncReadyCallback callback, gpointer data)
{
    gchar buf[64];
    g_snprintf(buf, sizeof(buf), "host-usb:forward:tcp:%u;tcp:%u", local,
               remote);
    aml_adb_command(buf, callback, data);
}
