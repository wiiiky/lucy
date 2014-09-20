/*
 * aml.c
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

#include "amlmainwindow.h"


static GtkWidget *createApplicationPage()
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *scrolledWidnow = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(box), scrolledWidnow, TRUE, TRUE, 0);
    GtkListStore *appStore = gtk_list_store_new(4,
                                                GDK_TYPE_PIXBUF,
                                                G_TYPE_STRING,
                                                G_TYPE_STRING,
                                                G_TYPE_FLOAT);
    GtkWidget *appList =
        gtk_tree_view_new_with_model(GTK_TREE_MODEL(appStore));
    gtk_container_add(GTK_CONTAINER(scrolledWidnow), appList);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(appList), FALSE);
    gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(appList),
                                 GTK_TREE_VIEW_GRID_LINES_VERTICAL);

    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;
    renderer = gtk_cell_renderer_pixbuf_new();
    column = gtk_tree_view_column_new_with_attributes("Icon",
                                                      renderer,
                                                      "pixbuf",
                                                      0,
                                                      "xalign", 3, NULL);
    gtk_tree_view_column_set_fixed_width(GTK_TREE_VIEW_COLUMN(column),
                                         100);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appList), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Application Name",
                                                      renderer,
                                                      "text",
                                                      1,
                                                      "xalign", 3, NULL);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appList), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Version",
                                                      renderer,
                                                      "text",
                                                      2,
                                                      "xalign", 3, NULL);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appList), column);


    int i;
    for (i = 0; i < 50; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(GTK_LIST_STORE(appStore), &iter);
        gtk_list_store_set(GTK_LIST_STORE(appStore), &iter,
                           0, NULL,
                           1, "Android Manager for Android",
                           2, "0.01", 3, 0.5, -1);
    }


    return box;
}


int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    adb_sysdeps_init();
    adb_trace_init();
/*    return adb_commandline(argc - 1, argv + 1);*/

    AmlMainWindow *window = aml_main_window_new();

    aml_main_window_show(window);

    return 0;
}
