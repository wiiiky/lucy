/*
 * lucy.c
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
#define  TRACE_TAG   TRACE_ADB

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

#include "libadb/sysdeps.h"
#include "libadb/adb.h"
#include "libadb/adb_auth.h"
#include "libadb/usb_vendors.h"

#include "ui_mainwindow.h"
#include "lcutil.h"


int main(int argc, char **argv)
{
    adb_sysdeps_init();
    adb_trace_init();
    if (g_strcmp0(argv[0], "adb") == 0) {
        /*
         * if argv[0] is "adb", lucy runs as adb
         */
        return adb_commandline(argc - 1, argv + 1);
    }
    lc_init(argc, argv);
    gtk_init(&argc, &argv);

    UIMainWindow *window = ui_main_window_new();
    ui_main_window_show(window);

    return 0;
}
