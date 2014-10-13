/*
 * lcmainwindow.c
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


#include "lcnotify.h"
#include "lcmainwindow.h"
#include "lctoolstack.h"
#include "lcapplicationview.h"
#include "lcaboutdialog.h"
#include "lcmyandroid.h"
#include "lcadb.h"
#include "lcsocket.h"
#include "lcutil.h"
#include "lccommander.h"
#include "lcinstalldialog.h"
#include "libadb/sysdeps.h"
#include "libadb/adb_client.h"
#include <time.h>

#define MAINWINDOW_TITLE "Android Device Manager - Lucy"

#define MY_ANDROID_TITLE    "My Android"
#define APPLICATION_VIEW_TITLE  "Applications"

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

#define LC_MAIN_WINDOW_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_LC_MAIN_WINDOW, LcMainWindowPrivate))

static gpointer lc_main_window_parent_class = NULL;

#define APPLICATION_VIEW_NAME "app"
#define APPLICATION_VIEW_TITLE "Applications"
#define SMS_VIEW_NAME   "sms"
#define SMS_VIEW_TITLE  "SMS"

static void lc_main_window_finalize(GObject * obj);

/* create the menu bar, of course along with all menus */
static GtkWidget *lc_main_window_menu_bar(LcMainWindow * self);


enum {
    LC_MAIN_WINDOW_DUMMY_PROPERTY
};

typedef enum {
    LC_PHONE_STATE_DISCONNECTED,
    LC_PHONE_STATE_CONNECTED,
    LC_PHONE_STATE_CONNECTING,
} LcPhoneState;

struct _LcMainWindowPrivate {
    LcToolStack *tool_stack;
    LcMyAndroid *phone;
    LcApplicationView *app_view;
    GtkMenu *app_popmenu;

    LcPhoneState state;
};

#define lc_main_window_get_application_view(self) ((self)->priv->app_view)
#define lc_main_window_get_tool_stack(self)     ((self)->priv->tool_stack)
#define lc_main_window_get_my_android(self)       ((self)->priv->phone)

#define lc_main_window_set_phone_state(self,_state)    ((self)->priv->state=_state)

#define lc_main_window_set_phone_connected(self)    lc_main_window_set_phone_state(self,LC_PHONE_STATE_CONNECTED)
#define lc_main_window_set_phone_disconnected(self) lc_main_window_set_phone_state(self,LC_PHONE_STATE_DISCONNECTED)
#define lc_main_window_set_phone_connecting(self)   lc_main_window_set_phone_state(self,LC_PHONE_STATE_CONNECTING)

#define lc_main_window_is_connected(self)   ((self)->priv->state==LC_PHONE_STATE_CONNECTED)

LcMainWindow *lc_main_window_construct(GType object_type)
{
    LcMainWindow *self = NULL;
    self = (LcMainWindow *) g_object_new(object_type, NULL);
    return self;
}


LcMainWindow *lc_main_window_new(void)
{
    return lc_main_window_construct(TYPE_LC_MAIN_WINDOW);
}


static void lc_main_window_class_init(LcMainWindowClass * klass)
{
    lc_main_window_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(LcMainWindowPrivate));
    G_OBJECT_CLASS(klass)->finalize = lc_main_window_finalize;
}

static void on_my_android(gboolean visible, gpointer user_data)
{
    if (visible) {
        g_message("Come on, My Android!");
    }
}

static void on_command_applications(const gchar * cmd, GByteArray * array,
                                    gpointer user_data)
{
    LcMainWindow *self = (LcMainWindow *) user_data;
    LcApplicationView *appView = lc_main_window_get_application_view(self);
    gchar *result = lc_util_get_string_from_byte_array(array, NULL);
    if (result == NULL || lc_protocol_get_result_from_string(result) !=
        LC_PROTOCOL_RESULT_OKAY) {
        g_warning("Command '%s' Failed:%s", cmd, result);
    } else {
        GList *list =
            lc_protocol_create_application_list(result +
                                                LC_PROTOCOL_HDR_LEN);
        lc_application_view_update(appView, list);
        lc_protocol_free_application_list(list);
    }
    g_free(result);
    lc_application_view_set_loading(appView, FALSE);
}

static void on_application(gboolean visible, gpointer user_data)
{
    LcMainWindow *self = (LcMainWindow *) user_data;
    LcApplicationView *view = lc_main_window_get_application_view(self);

    guint64 last = lc_application_view_get_update_time(view);
    guint64 now = (guint64) time(NULL);

    if (visible && lc_main_window_is_connected(self) &&
        lc_application_view_is_loading(view) == FALSE &&
        now - last >= 60) {
        lc_application_view_set_loading(view, TRUE);
        lc_commander_send_command_async(LC_PROTOCOL_APPLICATIONS,
                                        on_command_applications,
                                        user_data);
    }
}

static void lc_main_window_my_android_init(LcMainWindow * self);
static void lc_main_window_application_init(LcMainWindow * self);

static void lc_main_window_instance_init(LcMainWindow * self)
{
    self->priv = LC_MAIN_WINDOW_GET_PRIVATE(self);

    gtk_window_set_resizable(GTK_WINDOW(self), FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(self), 900, 600);
    gtk_window_set_position(GTK_WINDOW(self), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(self), MAINWINDOW_TITLE);
    g_signal_connect(G_OBJECT(self), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *root_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(self), root_box);

    gtk_box_pack_start(GTK_BOX(root_box), lc_main_window_menu_bar(self),
                       FALSE, FALSE, 0);

    self->priv->tool_stack = lc_tool_stack_new();
    gtk_box_pack_start(GTK_BOX(root_box),
                       GTK_WIDGET(self->priv->tool_stack), TRUE, TRUE, 0);
    g_object_ref_sink(self->priv->tool_stack);

    lc_main_window_my_android_init(self);
    lc_tool_stack_append(self->priv->tool_stack,
                         gtk_image_new_from_file
                         (lc_util_get_resource_by_name(MY_ANDROID_TITLE_ICON)),
                         MY_ANDROID_TITLE, GTK_WIDGET(self->priv->phone),
                         on_my_android, self);

    lc_main_window_application_init(self);
    lc_tool_stack_append(self->priv->tool_stack,
                         gtk_image_new_from_file
                         (lc_util_get_resource_by_name(APPLICATION_VIEW_TITLE_ICON)),
                         APPLICATION_VIEW_TITLE,
                         GTK_WIDGET(self->priv->app_view), on_application,
                         self);

    lc_main_window_set_phone_disconnected(self);
}

static void on_connect_clicked(GtkWidget * button, gpointer data)
{
    LcMainWindow *self = (LcMainWindow *) data;
    lc_my_android_show_connecting(self->priv->phone);
    lc_main_window_start_server(self);
    lc_main_window_set_phone_connecting(self);
}

static void lc_main_window_my_android_init(LcMainWindow * self)
{
    LcMyAndroid *phone = lc_my_android_new();
    lc_my_android_set_connect_callback(phone,
                                       G_CALLBACK(on_connect_clicked),
                                       self);
    g_object_ref_sink(phone);

    self->priv->phone = phone;
}

static void lc_main_window_application_init(LcMainWindow * self)
{
    self->priv->app_view = lc_application_view_new();
    g_object_ref_sink(self->priv->app_view);

    self->priv->app_popmenu = (GtkMenu *) gtk_menu_new();
    g_object_ref_sink(self->priv->app_popmenu);
    /* TODO */
}

static void lc_main_window_finalize(GObject * obj)
{
    LcMainWindow *self =
        G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_LC_MAIN_WINDOW,
                                   LcMainWindow);
    _g_object_unref0(self->priv->app_view);
    _g_object_unref0(self->priv->tool_stack);
    _g_object_unref0(self->priv->phone);
    _g_object_unref0(self->priv->app_popmenu);
    G_OBJECT_CLASS(lc_main_window_parent_class)->finalize(obj);
}


GType lc_main_window_get_type(void)
{
    static volatile gsize lc_main_window_type_id__volatile = 0;
    if (g_once_init_enter(&lc_main_window_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(LcMainWindowClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) lc_main_window_class_init,
            (GClassFinalizeFunc) NULL,
            NULL, sizeof(LcMainWindow), 0,
            (GInstanceInitFunc) lc_main_window_instance_init, NULL
        };
        GType lc_main_window_type_id;
        lc_main_window_type_id =
            g_type_register_static(GTK_TYPE_WINDOW, "LcMainWindow",
                                   &g_define_type_info, 0);
        g_once_init_leave(&lc_main_window_type_id__volatile,
                          lc_main_window_type_id);
    }
    return lc_main_window_type_id__volatile;
}

static void on_about_menu_item_activate(GtkMenuItem * item, gpointer data);
static void on_app_install_menu_item_activate(GtkMenuItem * item,
                                              gpointer data);
/* create menu bar, along with all menus */
static GtkWidget *lc_main_window_menu_bar(LcMainWindow * self)
{
    GtkWidget *menu_bar = gtk_menu_bar_new();

    /* APP */
    GtkWidget *app_item = gtk_menu_item_new_with_mnemonic("_APP");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), app_item);
    GtkWidget *app_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(app_item), app_menu);
    app_item = gtk_menu_item_new_with_mnemonic("_Install");
    gtk_menu_shell_append(GTK_MENU_SHELL(app_menu), app_item);
    g_signal_connect(G_OBJECT(app_item), "activate",
                     G_CALLBACK(on_app_install_menu_item_activate), self);

    /* ABOUT */
    GtkWidget *about_item = gtk_menu_item_new_with_mnemonic("_About");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), about_item);

    GtkWidget *about_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(about_item), about_menu);

    GtkWidget *_about_item = gtk_menu_item_new_with_mnemonic("_About");
    gtk_menu_shell_append(GTK_MENU_SHELL(about_menu), _about_item);
    g_signal_connect(G_OBJECT(_about_item), "activate",
                     G_CALLBACK(on_about_menu_item_activate), NULL);

    return menu_bar;
}

/* show LcAboutDialog */
static void on_about_menu_item_activate(GtkMenuItem * item, gpointer data)
{
    LcAboutDialog *dialog = lc_about_dialog_new();
    lc_about_dialog_run(dialog);
    lc_about_dialog_destroy(dialog);
}

/* install a new app */
static void on_app_install_menu_item_activate(GtkMenuItem * item,
                                              gpointer data)
{
    LcMainWindow *self = (LcMainWindow *) data;
    LcInstallDialog *dialog = lc_install_dialog_new(GTK_WINDOW(self));
    lc_install_dialog_run(dialog);
}

void lc_main_window_show(LcMainWindow * window)
{
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();
}

static void on_command_phone(const gchar * cmd, GByteArray * array,
                             gpointer user_data)
{
    gchar *result = lc_util_get_string_from_byte_array(array, NULL);
    if (result == NULL
        || lc_protocol_get_result_from_string(result) !=
        LC_PROTOCOL_RESULT_OKAY) {
        g_warning("Command '%s' Failed:%s", LC_PROTOCOL_PHONE, result);
    } else {
        LcMainWindow *self = (LcMainWindow *) user_data;
        LcProtocolPhone *phone =
            lc_protocol_create_phone(result + LC_PROTOCOL_HDR_LEN);
        lc_my_android_show_connected_with_info(self->priv->phone, phone);
        lc_protocol_phone_free(phone);
    }
    g_free(result);
}


static void on_connection_init(LcCommanderInitResult result, gpointer data)
{
    LcMainWindow *self = (LcMainWindow *) data;
    if (result == LC_COMMANDER_INIT_OK) {
        lc_commander_send_command_async(LC_PROTOCOL_PHONE,
                                        on_command_phone, data);
        lc_my_android_show_connected(self->priv->phone);
        lc_main_window_set_phone_connected(self);

        const gchar *title =
            lc_tool_stack_get_current_title(self->priv->tool_stack);
        if (g_strcmp0(title, APPLICATION_VIEW_TITLE) == 0) {
            on_application(true, self);
        }
        lc_notify_show("Lucy", "Connection established successfully!", CONNECT_SUCCESS_ICON);
    } else {
        /* Connection failed */
        lc_my_android_show_disconnect(self->priv->phone);
        lc_main_window_set_phone_disconnected(self);
        lc_notify_show("Lucy",
                       "Failed to connect to your Android device!", CONNECT_FAIL_ICON);
    }
}

void lc_main_window_start_server(LcMainWindow * window)
{
    lc_commander_init_async(on_connection_init, window);
}
