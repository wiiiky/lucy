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
#include "wrapper.h"
#include "util.h"
#include "adb_client.h"
#include "adb_auth.h"
#include "usb_vendors.h"
// #include "adb_trace.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>




int install_app(transport_type transport, char* serial, int argc, char** argv);

/*
 * 启动adb守护进程
 * 返回1表示成功，0表示失败
 */
int start_server(unsigned short port) {
    return launch_server(port) == 0;
}

/*
 * 关闭adb守护进程
 */
int kill_server(unsigned short port) {
    adb_set_tcp_specifics(port);
    return _adb_connect("host:kill") != -1;
}

/* 安装apk，r表示重新安装 */
int install_apk(const char *path, int r, unsigned short port) {
    adb_set_tcp_specifics(port);
    int argc = 2;
    char *argv[3] = {
        "install",
        (char*)path,
    };
    if(r) {
        argc = 3;
        argv[1] = "-r";
        argv[2] = (char*)path;
    }
    return install_app(kTransportUsb, NULL, argc, argv) == 0;
}

int forward_tcp(unsigned short local, unsigned short remote, int rebind, unsigned short port) {
    adb_set_tcp_specifics(port);
    char buf[128];
    snprintf(buf, sizeof(buf), "host-usb:forward%s:tcp:%u;tcp:%u",rebind?"":":norebind", local,remote);
    return adb_command(buf)==0;
}

int forward_remove_tcp(unsigned short local, unsigned short port) {
    adb_set_tcp_specifics(port);
    char buf[128];
    snprintf(buf, sizeof(buf), "host-usb:killforward:tcp:%u",local);
    return adb_command(buf)==0;
}

char *forward_list(unsigned short port) {
    adb_set_tcp_specifics(port);
    return adb_query("host-usb:list-forward");
}


/*********************************************
 * ADBD
 ********************************************/

static void *fdevent_loop_thread(void *data){
    fdevent_loop();
}

int adb_init(void){
    atexit(usb_cleanup);
    signal(SIGPIPE, SIG_IGN);

    init_transport_registration();
    usb_vendors_init();
    usb_init();
    local_init(DEFAULT_ADB_LOCAL_TRANSPORT_PORT);
    adb_auth_init();

    char local_name[30];
    build_local_name(local_name, sizeof(local_name), 5545);
    if(install_listener(local_name, "*smartsocket*", NULL, 0)) {
        return 0;
    }
    pthread_t tid;
    if(pthread_create(&tid, NULL, fdevent_loop_thread,NULL)){
        return 0;
    }


    return 1;
}

void adb_devices(char *buffer, unsigned int size, int use_long){
    memset(buffer, 0, size);
    list_transports(buffer, size, use_long);
}
