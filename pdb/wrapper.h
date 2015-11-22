/*
 * Copyright (C) 2015 Wiky L
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */
#ifndef __PYADB_WRAPPER_H__
#define __PYADB_WRAPPER_H__

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fdevent.h>
#include "adb.h"


int adb_init(void);
void adb_devices(char *buffer, unsigned int size,int use_long);


/*
 * 启动adb守护进程
 * 返回1表示成功，0表示失败
 */
int start_server(unsigned short port);


/*
 * 关闭adb守护进程
 */
int kill_server(unsigned short port);

/* 安装apk，r表示重新安装 */
int install_apk(const char *path, int r, unsigned short port);

/*
 *
 */
int forward_tcp(unsigned short local, unsigned short remote, int rebind, unsigned short port);
int forward_remove_tcp(unsigned short local, unsigned short port);

char *forward_list(unsigned short port);


#endif
