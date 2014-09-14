/*
 * gadb.c
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
#include <gtk/gtk.h>

#include "sysdeps.h"
#include "adb.h"
#include "adb_auth.h"
#include "usb_vendors.h"

#define PROGRAM_NAME "GADB"
#define GADB_WEBSITE "https://github.com/wiiiky/gadb"
#define GADB_COMMENTS "Android Manager for Linux"
#define GADB_COPYRIGH "Copyright (c) Wiky L"
#define GADB_VERSION "0.1"
const static gchar *GADB_AUTHORS[] = {
    "Wiky L (wiiiky@yeah.net)",
    NULL
};

static void onAboutMenuItemActivate(GtkMenuItem * item, gpointer data);

static GtkWidget *createMenuBar()
{
    GtkWidget *menuBar = gtk_menu_bar_new();
    GtkWidget *aboutItem = gtk_menu_item_new_with_mnemonic("_About");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), aboutItem);

    GtkWidget *aboutMenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(aboutItem), aboutMenu);

    GtkWidget *_aboutItem = gtk_menu_item_new_with_mnemonic("_About");
    gtk_menu_shell_append(GTK_MENU_SHELL(aboutMenu), _aboutItem);
    g_signal_connect(G_OBJECT(_aboutItem), "activate",
                     G_CALLBACK(onAboutMenuItemActivate), NULL);

    return menuBar;
}


static void onAboutMenuItemActivate(GtkMenuItem * item, gpointer data)
{
    GtkWidget *dialog = gtk_about_dialog_new();
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog),
                                      PROGRAM_NAME);
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), GADB_VERSION);
    gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),
                                      GTK_LICENSE_GPL_3_0);
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),
                                   GADB_COPYRIGH);
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), GADB_COMMENTS);
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), GADB_WEBSITE);
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(dialog), GADB_AUTHORS);

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);
}


int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    adb_sysdeps_init();
    adb_trace_init();
/*    return adb_commandline(argc - 1, argv + 1);*/

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 960, 640);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "GADB");
    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *rootBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), rootBox);

    gtk_box_pack_start(GTK_BOX(rootBox), createMenuBar(), FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
