/*
 * ui_mainwindow.c
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
#include "ui_mainwindow.h"
#include "ui_toolstack.h"
#include "ui_applicationview.h"
#include "ui_aboutdialog.h"
#include "ui_myandroid.h"
#include "ui_smsview.h"
#include "lcadb.h"
#include "lcsocket.h"
#include "lcutil.h"
#include "lccommander.h"
#include "ui_installdialog.h"
#include "libadb/sysdeps.h"
#include "libadb/adb_client.h"
#include <time.h>

#define MAINWINDOW_TITLE "Android Device Manager - Lucy"

#define MY_ANDROID_TITLE    "My Android"
#define APPLICATION_VIEW_TITLE "Applications"
#define SMS_VIEW_TITLE  	"SMS"

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

#define UI_MAIN_WINDOW_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_UI_MAIN_WINDOW, UIMainWindowPrivate))

static gpointer ui_main_window_parent_class = NULL;

static void ui_main_window_finalize(GObject * obj);

/* create the menu bar, of course along with all menus */
static GtkWidget *ui_main_window_menu_bar(UIMainWindow * self);


enum {
    UI_MAIN_WINDOW_DUMMY_PROPERTY
};

typedef enum {
    PHONE_STATE_DISCONNECTED,
    PHONE_STATE_CONNECTED,
    PHONE_STATE_CONNECTING,
} PhoneState;

struct _UIMainWindowPrivate {
    UIToolStack *tool_stack;
    UIMyAndroid *phone;
    UIApplicationView *app_view;
    UISMSView *sms_view;
    GtkMenu *app_popmenu;

    /* 当前处理状态 */
    gboolean loading_sms;       /* 当前是否在处理短信任务 */
    gboolean loading_app;       /* 当前是否在处理应用程序列表任务 */

    PhoneState state;
};

#define lc_main_window_set_phone_state(self,_state)    ((self)->priv->state=_state)
#define lc_main_window_set_phone_connected(self)    lc_main_window_set_phone_state(self,PHONE_STATE_CONNECTED)
#define lc_main_window_set_phone_disconnected(self) lc_main_window_set_phone_state(self,PHONE_STATE_DISCONNECTED)
#define lc_main_window_set_phone_connecting(self)   lc_main_window_set_phone_state(self,PHONE_STATE_CONNECTING)
#define lc_main_window_is_connected(self)   ((self)->priv->state==PHONE_STATE_CONNECTED)

UIMainWindow *ui_main_window_construct(GType object_type)
{
    UIMainWindow *self = NULL;
    self = (UIMainWindow *) g_object_new(object_type, NULL);
    return self;
}


UIMainWindow *ui_main_window_new(void)
{
    return ui_main_window_construct(TYPE_UI_MAIN_WINDOW);
}


static void ui_main_window_class_init(UIMainWindowClass * klass)
{
    ui_main_window_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(UIMainWindowPrivate));
    G_OBJECT_CLASS(klass)->finalize = ui_main_window_finalize;
}

/* 界面切换的事件 */
static void on_my_android_view(gboolean visible, gpointer user_data);
static void on_application_view(gboolean visible, gpointer user_data);
static void on_sms_view(gboolean visible, gpointer user_data);

/* 通信任务的回调 */
static void on_connection_init(LcCommanderInitResult result,
                               gpointer user_data);
static void on_command_phone(const gchar * cmd, GByteArray * array,
                             gpointer user_data);
static void on_command_applications(const gchar * cmd, GByteArray * array,
                                    gpointer user_data);
static void on_command_sms(const gchar * cmd, GByteArray * array,
                           gpointer user_data);

/* 各个界面的初始化 */
static void ui_main_window_my_android_init(UIMainWindow * self);
static void ui_main_window_application_init(UIMainWindow * self);
static void ui_main_window_sms_init(UIMainWindow * self);

static void ui_main_window_instance_init(UIMainWindow * self)
{
    self->priv = UI_MAIN_WINDOW_GET_PRIVATE(self);

    gtk_window_set_resizable(GTK_WINDOW(self), FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(self), 900, 600);
    gtk_window_set_position(GTK_WINDOW(self), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(self), MAINWINDOW_TITLE);
    g_signal_connect(G_OBJECT(self), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *root_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(self), root_box);

    gtk_box_pack_start(GTK_BOX(root_box), ui_main_window_menu_bar(self),
                       FALSE, FALSE, 0);

    self->priv->tool_stack = ui_tool_stack_new();
    gtk_box_pack_start(GTK_BOX(root_box),
                       GTK_WIDGET(self->priv->tool_stack), TRUE, TRUE, 0);
    g_object_ref_sink(self->priv->tool_stack);

    ui_main_window_my_android_init(self);
    ui_main_window_application_init(self);
    ui_main_window_sms_init(self);

    lc_main_window_set_phone_disconnected(self);
}

static void on_connect_clicked(GtkWidget * button, gpointer data)
{
    UIMainWindow *self = (UIMainWindow *) data;
    ui_my_android_show_connecting(self->priv->phone);
    ui_main_window_start_server(self);
    lc_main_window_set_phone_connecting(self);
}

static void ui_main_window_my_android_init(UIMainWindow * self)
{
    self->priv->phone = ui_my_android_new();
    ui_my_android_set_connect_callback(self->priv->phone,
                                       G_CALLBACK(on_connect_clicked),
                                       self);
    g_object_ref_sink(self->priv->phone);


    /* 添加 */
    ui_tool_stack_append(self->priv->tool_stack,
                         gtk_image_new_from_file
                         (lc_util_get_resource_by_name
                          (MY_ANDROID_TITLE_ICON)), MY_ANDROID_TITLE,
                         GTK_WIDGET(self->priv->phone), on_my_android_view,
                         self);
}

static void ui_main_window_application_init(UIMainWindow * self)
{
    self->priv->app_view = ui_application_view_new();
    g_object_ref_sink(self->priv->app_view);

    self->priv->app_popmenu = (GtkMenu *) gtk_menu_new();
    g_object_ref_sink(self->priv->app_popmenu);

    self->priv->loading_app = FALSE;

    /* 添加 */
    ui_tool_stack_append(self->priv->tool_stack,
                         gtk_image_new_from_file
                         (lc_util_get_resource_by_name
                          (APPLICATION_VIEW_TITLE_ICON)),
                         APPLICATION_VIEW_TITLE,
                         GTK_WIDGET(self->priv->app_view),
                         on_application_view, self);
}

static void ui_main_window_sms_init(UIMainWindow * self)
{
    self->priv->sms_view = ui_sms_view_new(NULL);
    g_object_ref_sink(self->priv->sms_view);

    self->priv->loading_sms = FALSE;
    /* 添加 */
    ui_tool_stack_append(self->priv->tool_stack,
                         gtk_image_new_from_file
                         (lc_util_get_resource_by_name
                          (SMS_VIEW_TITLE_ICON)),
                         SMS_VIEW_TITLE,
                         GTK_WIDGET(self->priv->sms_view), on_sms_view,
                         self);
}

static void ui_main_window_finalize(GObject * obj)
{
    UIMainWindow *self =
        G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_UI_MAIN_WINDOW,
                                   UIMainWindow);
    _g_object_unref0(self->priv->app_view);
    _g_object_unref0(self->priv->tool_stack);
    _g_object_unref0(self->priv->phone);
    _g_object_unref0(self->priv->sms_view);
    _g_object_unref0(self->priv->app_popmenu);
    G_OBJECT_CLASS(ui_main_window_parent_class)->finalize(obj);
}

static void on_my_android_view(gboolean visible, gpointer user_data)
{
    if (visible) {
        g_message("Come on, My Android!");
    }
}

static void on_sms_view(gboolean visible, gpointer user_data)
{
    UIMainWindow *self = (UIMainWindow *) user_data;
    UISMSView *view = self->priv->sms_view;

    if (visible && lc_main_window_is_connected(self)) {
        guint64 last = ui_sms_view_get_update_time(view);
        guint64 now = (guint64) time(NULL);
        if (now - last >= 60 && self->priv->loading_sms == FALSE) {
            self->priv->loading_sms = TRUE;
            lc_commander_send_command_async(LC_PROTOCOL_SMS,
                                            on_command_sms, user_data);
        }
    }
}

static void on_application_view(gboolean visible, gpointer user_data)
{
    UIMainWindow *self = (UIMainWindow *) user_data;
    UIApplicationView *view = self->priv->app_view;

    if (visible && lc_main_window_is_connected(self)) {
        guint64 last = ui_application_view_get_update_time(view);
        guint64 now = (guint64) time(NULL);

        if (now - last >= 60 && self->priv->loading_app == FALSE) {
            self->priv->loading_app = TRUE;
            lc_commander_send_command_async(LC_PROTOCOL_APPLICATIONS,
                                            on_command_applications,
                                            user_data);
        }
    }
}


GType ui_main_window_get_type(void)
{
    static volatile gsize ui_main_window_type_id__volatile = 0;
    if (g_once_init_enter(&ui_main_window_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(UIMainWindowClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) ui_main_window_class_init,
            (GClassFinalizeFunc) NULL,
            NULL, sizeof(UIMainWindow), 0,
            (GInstanceInitFunc) ui_main_window_instance_init, NULL
        };
        GType lc_main_window_type_id;
        lc_main_window_type_id =
            g_type_register_static(GTK_TYPE_WINDOW, "UIMainWindow",
                                   &g_define_type_info, 0);
        g_once_init_leave(&ui_main_window_type_id__volatile,
                          lc_main_window_type_id);
    }
    return ui_main_window_type_id__volatile;
}

/* 菜单事件 */
static void on_about_menu_item_activate(GtkMenuItem * item, gpointer data);
static void on_app_install_menu_item_activate(GtkMenuItem * item,
                                              gpointer data);
/* create menu bar, along with all menus */
static GtkWidget *ui_main_window_menu_bar(UIMainWindow * self)
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
    UIAboutDialog *dialog = ui_about_dialog_new();
    ui_about_dialog_run(dialog);
    ui_about_dialog_destroy(dialog);
}

/* install a new app */
static void on_app_install_menu_item_activate(GtkMenuItem * item,
                                              gpointer data)
{
    UIMainWindow *self = (UIMainWindow *) data;
    UIInstallDialog *dialog = ui_install_dialog_new(GTK_WINDOW(self));
    ui_install_dialog_run(dialog);
}

void ui_main_window_show(UIMainWindow * window)
{
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();
}

void ui_main_window_start_server(UIMainWindow * window)
{
    lc_commander_init_async(on_connection_init, window);
}

static void on_connection_init(LcCommanderInitResult result, gpointer data)
{
    UIMainWindow *self = (UIMainWindow *) data;
    if (result == LC_COMMANDER_INIT_OK) {
        lc_commander_send_command_async(LC_PROTOCOL_PHONE,
                                        on_command_phone, data);
        ui_my_android_show_connected(self->priv->phone);
        lc_main_window_set_phone_connected(self);

        const gchar *title =
            ui_tool_stack_get_current_title(self->priv->tool_stack);
        if (g_strcmp0(title, APPLICATION_VIEW_TITLE) == 0) {
            on_application_view(TRUE, self);
        } else if (g_strcmp0(title, SMS_VIEW_TITLE) == 0) {
            on_sms_view(TRUE, self);
        }
        lc_notify_show("Lucy", "Connection established successfully!",
                       CONNECT_SUCCESS_ICON);
    } else {
        /* Connection failed */
        ui_my_android_show_disconnect(self->priv->phone);
        lc_main_window_set_phone_disconnected(self);
        lc_notify_show("Lucy",
                       "Failed to connect to your Android device!",
                       CONNECT_FAIL_ICON);
    }
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
        UIMainWindow *self = (UIMainWindow *) user_data;
        LcProtocolPhone *phone =
            lc_protocol_create_phone(result + LC_PROTOCOL_HDR_LEN);
        ui_my_android_show_connected_with_info(self->priv->phone, phone);
        lc_protocol_phone_free(phone);
    }
    g_free(result);
}

static void on_command_sms(const gchar * cmd, GByteArray * array,
                           gpointer user_data)
{
    gchar *result = lc_util_get_string_from_byte_array(array, NULL);
    UIMainWindow *self = (UIMainWindow *) user_data;
    self->priv->loading_sms = FALSE;
    if (result == NULL ||
        lc_protocol_get_result_from_string(result) !=
        LC_PROTOCOL_RESULT_OKAY) {
        g_warning("Command '%s' Failed:%s", cmd, result);
    } else {
        GList *list =
            lc_protocol_create_sms_list(result + LC_PROTOCOL_HDR_LEN);
        ui_sms_view_update(self->priv->sms_view, list);
    }
    g_free(result);
}

static void on_command_applications(const gchar * cmd, GByteArray * array,
                                    gpointer user_data)
{
    UIMainWindow *self = (UIMainWindow *) user_data;
    self->priv->loading_app = FALSE;
    UIApplicationView *appView = self->priv->app_view;
    gchar *result = lc_util_get_string_from_byte_array(array, NULL);
    if (result == NULL || lc_protocol_get_result_from_string(result) !=
        LC_PROTOCOL_RESULT_OKAY) {
        g_warning("Command '%s' Failed:%s", cmd, result);
    } else {
        GList *list =
            lc_protocol_create_application_list(result +
                                                LC_PROTOCOL_HDR_LEN);
        ui_application_view_update(appView, list);
        lc_protocol_free_application_list(list);
    }
    g_free(result);
}
