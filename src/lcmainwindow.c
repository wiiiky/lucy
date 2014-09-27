/* lcmainwindow.c generated by valac 0.20.1, the Vala compiler
 * generated from lcmainwindow.vala, do not modify */


#include <glib.h>
#include <glib-object.h>
#include "lcmainwindow.h"
#include "lctoolstack.h"
#include "lcapplicationview.h"
#include "lcaboutdialog.h"
#include "lcmyphone.h"
#include "lcadb.h"
#include "lcsocket.h"
#include "lcutil.h"
#include "lccommander.h"
#include "libadb/sysdeps.h"
#include "libadb/adb_client.h"

#define MAINWINDOW_TITLE "Android Manager"

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

#define LC_MAIN_WINDOW_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_LC_MAIN_WINDOW, LcMainWindowPrivate))

static gpointer lc_main_window_parent_class = NULL;

#define APPLICATION_VIEW_NAME "app"
#define APPLICATION_VIEW_TITLE "Applications"
#define SMS_VIEW_NAME   "sms"
#define SMS_VIEW_TITLE  "SMS"

static void lc_main_window_finalize(GObject * obj);

/* 创建菜单栏 */
static GtkWidget *lc_main_window_menu_bar(LcMainWindow * self);

static void onAboutMenuItemActivate(GtkMenuItem * item, gpointer data);


enum {
    LC_MAIN_WINDOW_DUMMY_PROPERTY
};

struct _LcMainWindowPrivate {
    LcApplicationView *appView;
    LcToolStack *toolStack;
    LcMyPhone *myPhone;
};


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

static void onMyPhonePageVisible(gboolean visible, gpointer user_data)
{
    if (visible) {
        g_message("Come on, My Android!");
    }
}

static void lc_main_window_instance_init(LcMainWindow * self)
{
    self->priv = LC_MAIN_WINDOW_GET_PRIVATE(self);

    gtk_window_set_resizable(GTK_WINDOW(self), FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(self), 900, 600);
    //gtk_window_set_default_size(GTK_WINDOW(self), 960, 640);
    gtk_window_set_position(GTK_WINDOW(self), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(self), MAINWINDOW_TITLE);
    g_signal_connect(G_OBJECT(self), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *rootBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(self), rootBox);

    gtk_box_pack_start(GTK_BOX(rootBox), lc_main_window_menu_bar(self),
                       FALSE, FALSE, 0);

    self->priv->toolStack = lc_tool_stack_new();
    gtk_box_pack_start(GTK_BOX(rootBox),
                       GTK_WIDGET(self->priv->toolStack), TRUE, TRUE, 0);
    g_object_ref_sink(self->priv->toolStack);

    self->priv->myPhone = lc_my_phone_new();
    g_object_ref_sink(self->priv->myPhone);
    lc_tool_stack_append(self->priv->toolStack,
                         gtk_image_new_from_file
                         (lc_util_get_resource_by_name("smartphone.svg")),
                         "My Phone", GTK_WIDGET(self->priv->myPhone),
                         onMyPhonePageVisible, self);

    self->priv->appView = lc_application_view_new();
    g_object_ref_sink(self->priv->appView);
    lc_tool_stack_append(self->priv->toolStack,
                         gtk_image_new_from_file
                         (lc_util_get_resource_by_name("computer.svg")),
                         "Applications", GTK_WIDGET(self->priv->appView),
                         NULL, NULL);
}

static void lc_main_window_finalize(GObject * obj)
{
    LcMainWindow *self =
        G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_LC_MAIN_WINDOW,
                                   LcMainWindow);
    _g_object_unref0(self->priv->appView);
    _g_object_unref0(self->priv->toolStack);
    _g_object_unref0(self->priv->myPhone);
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

/*创建菜单栏*/
static GtkWidget *lc_main_window_menu_bar(LcMainWindow * self)
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

/* show LcAboutDialog */
static void onAboutMenuItemActivate(GtkMenuItem * item, gpointer data)
{
    LcAboutDialog *dialog = lc_about_dialog_new();
    lc_about_dialog_run(dialog);
    lc_about_dialog_destroy(dialog);
}

void lc_main_window_show(LcMainWindow * window)
{
    gtk_widget_show_all(GTK_WIDGET(window));
    lc_main_window_start_server(window);
    gtk_main();
}

static void onApplications(GObject * source_object,
                           GAsyncResult * res, gpointer user_data)
{
    GByteArray *array = lc_commander_send_command_finish(res);
    gchar *result = lc_util_get_string_from_byte_array(array, NULL);
    if (result == NULL) {
        g_warning("Connection Problem while 'applications'");
        return;
    }
    if (lc_protocol_get_result_from_string(result) !=
        LC_PROTOCOL_RESULT_OKAY) {
        g_warning("Command 'applications' Failed:%s", result);
    } else {
        LcMainWindow *self = (LcMainWindow *) user_data;
        LcApplicationView *appView = self->priv->appView;
        GList *list = lc_protocol_create_application_list(result + 4);
        GList *lp = list;
        while (lp) {
            LcProtocolApplication *info =
                (LcProtocolApplication *) lp->data;
            lc_application_view_append(appView, info);
            g_message("%s", info->appName);
            lp = g_list_next(lp);
        }
        lc_protocol_free_application_list(list);
    }
    g_free(result);
}


static void onConnectionInit(LcCommanderInitResult result, gpointer data)
{
    g_message("%d", result);
    if (result == LC_COMMANDER_INIT_OK) {
        g_message("OK");
        lc_commander_send_command("applications\n", onApplications, data);
    }
}

void lc_main_window_start_server(LcMainWindow * window)
{
    lc_commander_init(onConnectionInit, window);
}
