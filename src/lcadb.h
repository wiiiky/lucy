/*
 * lcadb.h
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
#ifndef __LC_ADB_H__
#define __LC_ADB_H__

#include <gtk/gtk.h>

/*
 * wrapper for adb functions
 */


/*
 * asynchronously execute adb_command() using GTask.
 * buf is the parameters of adb_command()
 */
void lc_adb_command(const gchar * buf, GAsyncReadyCallback callback,
                    gpointer data);
int lc_adb_command_finish(GAsyncResult * result);

/*
 * asynchronously execute adb_connect() using GTask.
 * buf is the parameters of adb_connect().
 */
void lc_adb_connect(const gchar * buf, GAsyncReadyCallback callback,
                    gpointer data);
int lc_adb_connect_finish(GAsyncResult * result);


/*
 * start the adb server on host.
 */
void lc_adb_start_server(GAsyncReadyCallback callback, gpointer data);
#define lc_adb_start_server_finish(result) lc_adb_connect_finish(result)

/*
 * forward socket connections asynchronously
 */
void lc_adb_forward(guint local, guint remote,
                    GAsyncReadyCallback callback, gpointer data);
#define lc_adb_forward_finish(result) lc_adb_command_finish(result)

/*
 * start a specified Activity in Android asynchronously.
 * activity should be something like 'org.wl.ll/.MainActivity'
 */
void lc_adb_am_start(const gchar * activity,
                     GAsyncReadyCallback callback, gpointer data);
#define lc_adb_am_start_finish(result) lc_adb_connect_finish(result)

/*
 * install an apk file in Android asynchronously.
 */
void lc_adb_install_app(const gchar * filepath,
                        GAsyncReadyCallback callback, gpointer data);
int lc_adb_install_app_finish(GAsyncResult * result);

#endif
