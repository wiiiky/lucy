/*
 * amladb.h
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
#ifndef __AML_ADB_H__
#define __AML_ADB_H__

#include <gtk/gtk.h>

/*
 * adb 相关
 */


/*
 * 使用GTask异步执行adb_command
 * 执行完毕调用callback
 * XXX：这里的buf不是adb命令行的参数，而是adb源代码中adb_command函数的参数
 */
void aml_adb_command(const gchar * buf, GAsyncReadyCallback callback,
                     gpointer data);
int aml_adb_command_finish(GAsyncResult * result);

/*
 * 异步执行adb_connect
 */
void aml_adb_connect(const gchar * buf, GAsyncReadyCallback callback,
                     gpointer data);
int aml_adb_connect_finish(GAsyncResult * result);


/*
 * 启动adb服务器
 */
void aml_adb_start_server(GAsyncReadyCallback callback, gpointer data);
#define aml_adb_start_server_finish(result) aml_adb_connect_finish(result)

/*
 * 重定向tcp端口
 * aml_adb_command的简单封装
 */
void aml_adb_forward(guint local, guint remote,
                     GAsyncReadyCallback callback, gpointer data);
#define aml_adb_forward_finish(result) aml_adb_command_finish(result)

/*
 * 启动android机器上的指定程序
 */
void aml_adb_am_start(const gchar * activity,
                      GAsyncReadyCallback callback, gpointer data);
#define aml_adb_am_start_finish(result) aml_adb_connect_finish(result)

/*
 * 安装指定apk到android手机
 */
void aml_adb_install_app(const gchar * filepath,
                         GAsyncReadyCallback callback, gpointer data);
int aml_adb_install_app_finish(GAsyncResult * result);

#endif
