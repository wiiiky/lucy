/*
 * ui_applicationview.c
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


#include <gtk/gtk.h>
#include "ui_applicationview.h"
#include "ui_applicationrow.h"
#include "lcprotocol.h"
#include "lcadb.h"
#include "lcutil.h"
#include <time.h>

#define _g_list_free0(var) ((var == NULL) ? NULL : (var = (g_list_free (var), NULL)))
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

struct _UIApplicationViewPrivate {
    GtkGrid *app_grid;
    GtkToolbar *app_toolbar;
    GtkRadioToolButton *all_app;
    GtkRadioToolButton *sys_app;
    GtkRadioToolButton *third_app;
    GtkScrolledWindow *app_scrolled;
    GtkGrid *apps;
    gint app_count;

    gboolean app_loading;       /* loading data from LILY? */
    guint64 app_last_update;    /* the time in second that list updated last time */

    UIApplicationRow *selected_row; /* the pointer to the selected row */
};

#define STACK_NAME_APPLICATION_VIEW "application-view"


#define RADIO_BUTTON_ALL    "ALL"
#define RADIO_BUTTON_SYS    "SYSTEM ONLY"
#define RADIO_BUTTON_THIRD   "3RD PARTY ONLY"

#define lc_application_view_set_selected(self,row) \
        do{ \
        if(self->priv->selected_row){ \
            ui_application_row_unhighlight(self->priv->selected_row); \
        }   \
        self->priv->selected_row=(row); \
        ui_application_row_highlight(row); \
        }while(0)


static gpointer ui_application_view_parent_class = NULL;

#define UI_APPLICATION_VIEW_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_UI_APPLICATION_VIEW, UIApplicationViewPrivate))
enum {
    UI_APPLICATION_VIEW_DUMMY_PROPERTY
};
static void ui_application_view_finalize(GObject * obj);
static void on_radio_tool_button_toggled(GtkRadioToolButton * radio,
                                         gpointer user_data);
static inline void ui_application_view_show_all(UIApplicationView * self);
static inline void ui_application_view_show_system(UIApplicationView *
                                                   self);
static inline void ui_application_view_show_third(UIApplicationView *
                                                  self);


UIApplicationView *ui_application_view_construct(GType object_type)
{
    UIApplicationView *self = NULL;
    self = (UIApplicationView *) g_object_new(object_type, NULL);
    gtk_stack_add_named(GTK_STACK(self), GTK_WIDGET(self->priv->app_grid),
                        STACK_NAME_APPLICATION_VIEW);

    lc_util_load_css(GTK_WIDGET(self), APPLICATION_VIEW_CSS_FILE);
    return self;
}


UIApplicationView *ui_application_view_new(void)
{
    return ui_application_view_construct(TYPE_UI_APPLICATION_VIEW);
}


static void ui_application_view_class_init(UIApplicationViewClass * klass)
{
    ui_application_view_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(UIApplicationViewPrivate));
    G_OBJECT_CLASS(klass)->finalize = ui_application_view_finalize;
}


static void ui_application_view_instance_init(UIApplicationView * self)
{
    self->priv = UI_APPLICATION_VIEW_GET_PRIVATE(self);

    UIApplicationViewPrivate *priv = self->priv;
    priv->app_grid = (GtkGrid *) gtk_grid_new();
    gtk_grid_set_column_homogeneous(priv->app_grid, TRUE);
    g_object_ref_sink(priv->app_grid);

    priv->app_toolbar = (GtkToolbar *) gtk_toolbar_new();
    gtk_toolbar_set_style(priv->app_toolbar, GTK_TOOLBAR_TEXT);
    g_object_ref_sink(priv->app_toolbar);
    gtk_grid_attach(priv->app_grid, GTK_WIDGET(priv->app_toolbar), 0, 0, 1,
                    1);

    priv->all_app = (GtkRadioToolButton *) gtk_radio_tool_button_new(NULL);
    g_signal_connect(G_OBJECT(priv->all_app), "toggled",
                     G_CALLBACK(on_radio_tool_button_toggled), self);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->all_app),
                              RADIO_BUTTON_ALL);
    priv->sys_app = (GtkRadioToolButton *)
        gtk_radio_tool_button_new_from_widget(priv->all_app);
    g_signal_connect(G_OBJECT(priv->sys_app), "toggled",
                     G_CALLBACK(on_radio_tool_button_toggled), self);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->sys_app),
                              RADIO_BUTTON_SYS);
    priv->third_app = (GtkRadioToolButton *)
        gtk_radio_tool_button_new_from_widget(priv->all_app);
    g_signal_connect(G_OBJECT(priv->third_app), "toggled",
                     G_CALLBACK(on_radio_tool_button_toggled), self);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->third_app),
                              RADIO_BUTTON_THIRD);
    gtk_toolbar_insert(priv->app_toolbar, GTK_TOOL_ITEM(priv->all_app),
                       -1);
    gtk_toolbar_insert(priv->app_toolbar, GTK_TOOL_ITEM(priv->sys_app),
                       -1);
    gtk_toolbar_insert(priv->app_toolbar, GTK_TOOL_ITEM(priv->third_app),
                       -1);

    priv->app_scrolled =
        (GtkScrolledWindow *) gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(GTK_WIDGET(priv->app_scrolled), TRUE);
    g_object_ref_sink(priv->app_scrolled);
    gtk_grid_attach(priv->app_grid, GTK_WIDGET(priv->app_scrolled), 0, 1,
                    1, 1);

    priv->apps = (GtkGrid *) gtk_grid_new();
    gtk_widget_set_name(GTK_WIDGET(priv->apps), "apps");
    gtk_grid_set_column_homogeneous(priv->apps, TRUE);
    g_object_ref_sink(priv->apps);
    gtk_container_add(GTK_CONTAINER(priv->app_scrolled),
                      GTK_WIDGET(priv->apps));

    priv->app_count = 0;
    priv->app_loading = FALSE;
    priv->app_last_update = 0;
    priv->selected_row = NULL;
}


static void ui_application_view_finalize(GObject * obj)
{
    UIApplicationView *self;
    self =
        G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_UI_APPLICATION_VIEW,
                                   UIApplicationView);
    _g_object_unref0(self->priv->app_grid);
    _g_object_unref0(self->priv->app_toolbar);
    _g_object_unref0(self->priv->apps);
    _g_object_unref0(self->priv->app_scrolled);
    G_OBJECT_CLASS(ui_application_view_parent_class)->finalize(obj);
}


GType ui_application_view_get_type(void)
{
    static volatile gsize ui_application_view_type_id__volatile = 0;
    if (g_once_init_enter(&ui_application_view_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(UIApplicationViewClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) ui_application_view_class_init,
            (GClassFinalizeFunc) NULL, NULL, sizeof(UIApplicationView), 0,
            (GInstanceInitFunc) ui_application_view_instance_init, NULL
        };
        GType ui_application_view_type_id;
        ui_application_view_type_id =
            g_type_register_static(GTK_TYPE_STACK,
                                   "UIApplicationView",
                                   &g_define_type_info, 0);
        g_once_init_leave(&ui_application_view_type_id__volatile,
                          ui_application_view_type_id);
    }
    return ui_application_view_type_id__volatile;
}

static void on_radio_tool_button_toggled(GtkRadioToolButton * radio,
                                         gpointer user_data)
{
    if (gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(radio))) {
        const gchar *label =
            gtk_tool_button_get_label(GTK_TOOL_BUTTON(radio));
        UIApplicationView *self = (UIApplicationView *) user_data;
        if (g_strcmp0(label, RADIO_BUTTON_THIRD) == 0) {
            ui_application_view_show_third(self);
        } else if (g_strcmp0(label, RADIO_BUTTON_SYS) == 0) {
            ui_application_view_show_system(self);
        } else {
            ui_application_view_show_all(self);
        }
    }
}

static gint ui_application_view_get_row_number(UIApplicationView * self,
                                               UIApplicationRow * row)
{
    GtkGrid *grid = self->priv->apps;
    gint num = -1;
    gtk_container_child_get(GTK_CONTAINER(grid), GTK_WIDGET(row),
                            "top-attach", &num, NULL);
    return num;
}

static gboolean on_row_button_pressed(GtkWidget * widget,
                                      GdkEventButton * event,
                                      gpointer user_data)
{
    UIApplicationRow *row = (UIApplicationRow *) widget;
    UIApplicationView *self = (UIApplicationView *) user_data;
    lc_application_view_set_selected(self, row);
    return FALSE;
}

#define UNINSTALL_KEY_ROW  "uninstall-row"
#define UNINSTALL_KEY_SELF  "uninstall-self"
static void on_uninstall_app(GObject * source_object, GAsyncResult * res,
                             gpointer user_data)
{
    int ret = lc_adb_uninstall_app_finish(res);
    GtkButton *uninstall_button = (GtkButton *) user_data;
    if (ret) {
        gtk_button_set_label(uninstall_button, "uninstall");
        gtk_widget_set_sensitive(GTK_WIDGET(uninstall_button), TRUE);
    } else {
        UIApplicationRow *row = (UIApplicationRow *)
            g_object_get_data(G_OBJECT(uninstall_button),
                              UNINSTALL_KEY_ROW);
        UIApplicationView *self = (UIApplicationView *)
            g_object_get_data(G_OBJECT(uninstall_button),
                              UNINSTALL_KEY_SELF);
        gint count = ui_application_view_get_row_number(self, row);
        ui_application_view_remove_row(self, count);
    }
}

static void on_uninstall_button_clicked(GtkButton * button,
                                        gpointer user_data)
{
    gtk_button_set_label(button, "uninstalling...");
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
    UIApplicationRow *row =
        (UIApplicationRow *) g_object_get_data(G_OBJECT(button),
                                               UNINSTALL_KEY_ROW);
    LcProtocolApplication *info =
        (LcProtocolApplication *) ui_application_row_get_data(row);

    lc_adb_uninstall_app(info->package_name, on_uninstall_app, button);
}

void ui_application_view_append_row(UIApplicationView * self,
                                    LcProtocolApplication * info)
{
    gint count = self->priv->app_count;
    UIApplicationRow *row = ui_application_row_new_with_data(info);
    GtkButton *uninstall_button =
        ui_application_row_get_uninstall_button(row);
    g_object_set_data(G_OBJECT(uninstall_button), UNINSTALL_KEY_ROW,
                      (gpointer) (gulong) count);
    g_object_set_data(G_OBJECT(uninstall_button), UNINSTALL_KEY_ROW,
                      (gpointer) row);
    g_object_set_data(G_OBJECT(uninstall_button), UNINSTALL_KEY_SELF,
                      (gpointer) self);
    g_signal_connect(G_OBJECT(uninstall_button), "clicked",
                     G_CALLBACK(on_uninstall_button_clicked), self);
    g_signal_connect(G_OBJECT(row), "button-press-event",
                     G_CALLBACK(on_row_button_pressed), self);
    gtk_grid_attach(self->priv->apps, GTK_WIDGET(row), 0, count, 1, 1);
    self->priv->app_count++;
}

void ui_application_view_remove_row(UIApplicationView * self, gint row)
{
    GtkGrid *grid = self->priv->apps;
    if (gtk_grid_get_child_at(grid, 0, row)) {
        gtk_grid_remove_row(grid, row);
        self->priv->app_count--;
    }
}

void ui_application_view_update(UIApplicationView * self, GList * list)
{
    self->priv->app_last_update = (guint64) time(NULL);

    GList *copy = g_list_copy(list);
    gint i, max = self->priv->app_count;
    GtkGrid *grid = self->priv->apps;
    for (i = 0; i < max; i++) {
        UIApplicationRow *row =
            (UIApplicationRow *) gtk_grid_get_child_at(grid, 0, i);
        if (row) {
            LcProtocolApplication *info = ui_application_row_get_data(row);
            LcProtocolApplication *find =
                lc_protocol_application_find(list, info->package_name);
            if (find) {
                /* update */
                ui_application_row_update_data(row, find);
                copy = g_list_remove(copy, info);
            } else {
                /* not found, remove */
                ui_application_view_remove_row(self, i);
                i--;
            }
        } else {
            g_warning("WTF!! i= %d", i);
        }
    }

    GList *lp = copy;
    while (lp) {
        LcProtocolApplication *info = (LcProtocolApplication *) lp->data;
        ui_application_view_append_row(self, info);
        lp = g_list_next(lp);
    }
    g_list_free(copy);

    ui_application_view_update_view(self);
}

void ui_application_view_set_loading(UIApplicationView * self,
                                     gboolean loading)
{
    self->priv->app_loading = loading;
}

gboolean ui_application_view_is_loading(UIApplicationView * self)
{
    return self->priv->app_loading;
}

guint64 ui_application_view_get_update_time(UIApplicationView * self)
{
    return self->priv->app_last_update;
}

LcProtocolApplicationType
ui_application_view_get_current_type(UIApplicationView * self)
{
    GtkRadioToolButton *all_app = self->priv->all_app;
    GtkRadioToolButton *sys_app = self->priv->sys_app;
    GtkRadioToolButton *third_app = self->priv->third_app;
    if (gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(all_app))) {
        return LC_PROTOCOL_APPLICATION_TYPE_ALL;
    } else
        if (gtk_toggle_tool_button_get_active
            (GTK_TOGGLE_TOOL_BUTTON(sys_app))) {
        return LC_PROTOCOL_APPLICATION_TYPE_SYSTEM;
    } else
        if (gtk_toggle_tool_button_get_active
            (GTK_TOGGLE_TOOL_BUTTON(third_app))) {
        return LC_PROTOCOL_APPLICATION_TYPE_THIRD;
    }
    return LC_PROTOCOL_APPLICATION_TYPE_UNKNOWN;
}

void ui_application_view_update_view(UIApplicationView * self)
{
    LcProtocolApplicationType type =
        ui_application_view_get_current_type(self);
    if (type == LC_PROTOCOL_APPLICATION_TYPE_ALL) {
        ui_application_view_show_all(self);
    } else if (type == LC_PROTOCOL_APPLICATION_TYPE_SYSTEM) {
        ui_application_view_show_system(self);
    } else if (type == LC_PROTOCOL_APPLICATION_TYPE_THIRD) {
        ui_application_view_show_third(self);
    }
}

static inline void ui_application_view_show_all(UIApplicationView * self)
{
    gint i, max = self->priv->app_count;
    GtkGrid *apps = self->priv->apps;
    for (i = 0; i < max; i++) {
        UIApplicationRow *row =
            (UIApplicationRow *) gtk_grid_get_child_at(apps, 0, i);
        gtk_widget_show(GTK_WIDGET(row));
    }
}

static inline void ui_application_view_show_system(UIApplicationView *
                                                   self)
{
    gint i, max = self->priv->app_count;
    GtkGrid *apps = self->priv->apps;
    for (i = 0; i < max; i++) {
        UIApplicationRow *row =
            (UIApplicationRow *) gtk_grid_get_child_at(apps, 0, i);
        LcProtocolApplication *app = (LcProtocolApplication *)
            ui_application_row_get_data(row);
        if (app->type == LC_PROTOCOL_APPLICATION_TYPE_SYSTEM) {
            gtk_widget_show(GTK_WIDGET(row));
        } else {
            gtk_widget_hide(GTK_WIDGET(row));
        }
    }
}

static inline void ui_application_view_show_third(UIApplicationView * self)
{
    gint i, max = self->priv->app_count;
    GtkGrid *apps = self->priv->apps;
    for (i = 0; i < max; i++) {
        UIApplicationRow *row =
            (UIApplicationRow *) gtk_grid_get_child_at(apps, 0, i);
        LcProtocolApplication *app = (LcProtocolApplication *)
            ui_application_row_get_data(row);
        if (app->type == LC_PROTOCOL_APPLICATION_TYPE_THIRD) {
            gtk_widget_show(GTK_WIDGET(row));
        } else {
            gtk_widget_hide(GTK_WIDGET(row));
        }
    }
}
