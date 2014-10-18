/*
 * lcapplicationview.c
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
#include "lcapplicationview.h"
#include "lcapplicationrow.h"
#include "lcprotocol.h"
#include "lcadb.h"
#include "lcutil.h"
#include <time.h>

#define _g_list_free0(var) ((var == NULL) ? NULL : (var = (g_list_free (var), NULL)))
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

struct _LcApplicationViewPrivate {
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

    LcApplicationRow *selected_row; /* the pointer to the selected row */
};

#define STACK_NAME_APPLICATION_VIEW "application-view"


#define RADIO_BUTTON_ALL    "ALL"
#define RADIO_BUTTON_SYS    "SYSTEM ONLY"
#define RADIO_BUTTON_THIRD   "3RD PARTY ONLY"

#define lc_application_view_get_priv(self)	((self)->priv)
#define lc_application_view_get_grid(self) \
	lc_application_view_get_priv(self)->apps
#define lc_application_view_get_row_count(self) \
	lc_application_view_get_priv(self)->app_count
#define lc_application_view_set_row_count(self,count) \
	lc_application_view_get_row_count(self)=(count)
#define lc_application_view_inc_row_count(self) \
	lc_application_view_get_row_count(self)= \
	lc_application_view_get_row_count(self) + 1
#define lc_application_view_dec_row_count(self) \
	lc_application_view_get_row_count(self) = \
	lc_application_view_get_row_count(self) - 1
#define lc_application_view_get_loading(self) \
        lc_application_view_get_priv(self)->app_loading
#define lc_application_view_get_updatetime(self) \
        lc_application_view_get_priv(self)->app_last_update
#define lc_application_view_set_updatetime(self,time) \
        lc_application_view_get_updatetime(self)=time
#define lc_application_view_get_selected(self) \
        lc_application_view_get_priv(self)->selected_row
#define lc_application_view_set_selected(self,row) \
        do{ \
        if(lc_application_view_get_selected(self)){ \
            lc_application_row_unhighlight(lc_application_view_get_selected(self)); \
        }   \
        lc_application_view_get_selected(self)=(row); \
        lc_application_row_highlight(row); \
        }while(0)


static gpointer lc_application_view_parent_class = NULL;

#define LC_APPLICATION_VIEW_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_LC_APPLICATION_VIEW, LcApplicationViewPrivate))
enum {
    LC_APPLICATION_VIEW_DUMMY_PROPERTY
};
static void lc_application_view_finalize(GObject * obj);
static void on_radio_tool_button_toggled(GtkRadioToolButton * radio,
                                         gpointer user_data);
static inline void lc_application_view_show_all(LcApplicationView * self);
static inline void lc_application_view_show_system(LcApplicationView *
                                                   self);
static inline void lc_application_view_show_third(LcApplicationView *
                                                  self);


LcApplicationView *lc_application_view_construct(GType object_type)
{
    LcApplicationView *self = NULL;
    self = (LcApplicationView *) g_object_new(object_type, NULL);
    gtk_stack_add_named(GTK_STACK(self), GTK_WIDGET(self->priv->app_grid),
                        STACK_NAME_APPLICATION_VIEW);

    lc_util_load_css(GTK_WIDGET(self), APPLICATION_VIEW_CSS_FILE);
    return self;
}


LcApplicationView *lc_application_view_new(void)
{
    return lc_application_view_construct(TYPE_LC_APPLICATION_VIEW);
}


static void lc_application_view_class_init(LcApplicationViewClass * klass)
{
    lc_application_view_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(LcApplicationViewPrivate));
    G_OBJECT_CLASS(klass)->finalize = lc_application_view_finalize;
}


static void lc_application_view_instance_init(LcApplicationView * self)
{
    self->priv = LC_APPLICATION_VIEW_GET_PRIVATE(self);

    LcApplicationViewPrivate *priv = self->priv;
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
    priv->sys_app =
        (GtkRadioToolButton *)
        gtk_radio_tool_button_new_from_widget(priv->all_app);
    g_signal_connect(G_OBJECT(priv->sys_app), "toggled",
                     G_CALLBACK(on_radio_tool_button_toggled), self);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->sys_app),
                              RADIO_BUTTON_SYS);
    priv->third_app =
        (GtkRadioToolButton *)
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


static void lc_application_view_finalize(GObject * obj)
{
    LcApplicationView *self;
    self =
        G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_LC_APPLICATION_VIEW,
                                   LcApplicationView);
    _g_object_unref0(self->priv->app_grid);
    _g_object_unref0(self->priv->app_toolbar);
    _g_object_unref0(self->priv->apps);
    _g_object_unref0(self->priv->app_scrolled);
    G_OBJECT_CLASS(lc_application_view_parent_class)->finalize(obj);
}


GType lc_application_view_get_type(void)
{
    static volatile gsize lc_application_view_type_id__volatile = 0;
    if (g_once_init_enter(&lc_application_view_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(LcApplicationViewClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) lc_application_view_class_init,
            (GClassFinalizeFunc) NULL, NULL, sizeof(LcApplicationView), 0,
            (GInstanceInitFunc) lc_application_view_instance_init, NULL
        };
        GType lc_application_view_type_id;
        lc_application_view_type_id =
            g_type_register_static(GTK_TYPE_STACK,
                                   "LcApplicationView",
                                   &g_define_type_info, 0);
        g_once_init_leave(&lc_application_view_type_id__volatile,
                          lc_application_view_type_id);
    }
    return lc_application_view_type_id__volatile;
}

static void on_radio_tool_button_toggled(GtkRadioToolButton * radio,
                                         gpointer user_data)
{
    if (gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(radio))) {
        const gchar *label =
            gtk_tool_button_get_label(GTK_TOOL_BUTTON(radio));
        LcApplicationView *self = (LcApplicationView *) user_data;
        if (g_strcmp0(label, RADIO_BUTTON_THIRD) == 0) {
            lc_application_view_show_third(self);
        } else if (g_strcmp0(label, RADIO_BUTTON_SYS) == 0) {
            lc_application_view_show_system(self);
        } else {
            lc_application_view_show_all(self);
        }
    }
}

static gint lc_application_view_get_row_number(LcApplicationView * self,
                                               LcApplicationRow * row)
{
    GtkGrid *grid = lc_application_view_get_grid(self);
    gint num = -1;
    gtk_container_child_get(GTK_CONTAINER(grid), GTK_WIDGET(row),
                            "top-attach", &num, NULL);
    return num;
}

static gboolean on_row_button_pressed(GtkWidget * widget,
                                      GdkEventButton * event,
                                      gpointer user_data)
{
    LcApplicationRow *row = (LcApplicationRow *) widget;
    LcApplicationView *self = (LcApplicationView *) user_data;
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
        LcApplicationRow *row = (LcApplicationRow *)
            g_object_get_data(G_OBJECT(uninstall_button),
                              UNINSTALL_KEY_ROW);
        LcApplicationView *self = (LcApplicationView *)
            g_object_get_data(G_OBJECT(uninstall_button),
                              UNINSTALL_KEY_SELF);
        gint count = lc_application_view_get_row_number(self, row);
        lc_application_view_remove_row(self, count);
    }
}

static void on_uninstall_button_clicked(GtkButton * button,
                                        gpointer user_data)
{
    gtk_button_set_label(button, "uninstalling...");
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
    LcApplicationRow *row =
        (LcApplicationRow *) g_object_get_data(G_OBJECT(button),
                                               UNINSTALL_KEY_ROW);
    LcProtocolApplication *info =
        (LcProtocolApplication *) lc_application_row_get_data(row);

    lc_adb_uninstall_app(info->package_name, on_uninstall_app, button);
}

void lc_application_view_append_row(LcApplicationView * self,
                                    LcProtocolApplication * info)
{
    gint count = lc_application_view_get_row_count(self);
    LcApplicationRow *row = lc_application_row_new_with_data(info);
    GtkButton *uninstall_button =
        lc_application_row_get_uninstall_button(row);
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
    gtk_grid_attach(lc_application_view_get_grid(self), GTK_WIDGET(row), 0,
                    count, 1, 1);
    lc_application_view_inc_row_count(self);
}

void lc_application_view_remove_row(LcApplicationView * self, gint row)
{
    GtkGrid *grid = lc_application_view_get_grid(self);
    if (gtk_grid_get_child_at(grid, 0, row)) {
        gtk_grid_remove_row(grid, row);
        lc_application_view_dec_row_count(self);
    }
}

void lc_application_view_update(LcApplicationView * self, GList * list)
{
    lc_application_view_set_updatetime(self, (guint64) time(NULL));

    GList *copy = g_list_copy(list);
    gint i, max = lc_application_view_get_row_count(self);
    GtkGrid *grid = lc_application_view_get_grid(self);
    for (i = 0; i < max; i++) {
        LcApplicationRow *row =
            (LcApplicationRow *) gtk_grid_get_child_at(grid, 0, i);
        if (row) {
            LcProtocolApplication *info = lc_application_row_get_data(row);
            LcProtocolApplication *find =
                lc_protocol_application_find(list, info->package_name);
            if (find) {
                /* update */
                lc_application_row_update_data(row, find);
                copy = g_list_remove(copy, info);
            } else {
                /* not found, remove */
                lc_application_view_remove_row(self, i);
                i--;
            }
        } else {
            g_warning("WTF!! i= %d", i);
        }
    }

    GList *lp = copy;
    while (lp) {
        LcProtocolApplication *info = (LcProtocolApplication *) lp->data;
        lc_application_view_append_row(self, info);
        lp = g_list_next(lp);
    }
    g_list_free(copy);

    lc_application_view_update_view(self);
}

void lc_application_view_set_loading(LcApplicationView * self,
                                     gboolean loading)
{
    lc_application_view_get_loading(self) = loading;
}

gboolean lc_application_view_is_loading(LcApplicationView * self)
{
    return lc_application_view_get_loading(self);
}

guint64 lc_application_view_get_update_time(LcApplicationView * self)
{
    return lc_application_view_get_updatetime(self);
}

LcProtocolApplicationType
lc_application_view_get_current_type(LcApplicationView * self)
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

void lc_application_view_update_view(LcApplicationView * self)
{
    LcProtocolApplicationType type =
        lc_application_view_get_current_type(self);
    if (type == LC_PROTOCOL_APPLICATION_TYPE_ALL) {
        lc_application_view_show_all(self);
    } else if (type == LC_PROTOCOL_APPLICATION_TYPE_SYSTEM) {
        lc_application_view_show_system(self);
    } else if (type == LC_PROTOCOL_APPLICATION_TYPE_THIRD) {
        lc_application_view_show_third(self);
    }
}

static inline void lc_application_view_show_all(LcApplicationView * self)
{
    gint i, max = self->priv->app_count;
    GtkGrid *apps = self->priv->apps;
    for (i = 0; i < max; i++) {
        LcApplicationRow *row =
            (LcApplicationRow *) gtk_grid_get_child_at(apps, 0, i);
        gtk_widget_show(GTK_WIDGET(row));
    }
}

static inline void lc_application_view_show_system(LcApplicationView *
                                                   self)
{
    gint i, max = self->priv->app_count;
    GtkGrid *apps = self->priv->apps;
    for (i = 0; i < max; i++) {
        LcApplicationRow *row =
            (LcApplicationRow *) gtk_grid_get_child_at(apps, 0, i);
        LcProtocolApplication *app = (LcProtocolApplication *)
            lc_application_row_get_data(row);
        if (app->type == LC_PROTOCOL_APPLICATION_TYPE_SYSTEM) {
            gtk_widget_show(GTK_WIDGET(row));
        } else {
            gtk_widget_hide(GTK_WIDGET(row));
        }
    }
}

static inline void lc_application_view_show_third(LcApplicationView * self)
{
    gint i, max = self->priv->app_count;
    GtkGrid *apps = self->priv->apps;
    for (i = 0; i < max; i++) {
        LcApplicationRow *row =
            (LcApplicationRow *) gtk_grid_get_child_at(apps, 0, i);
        LcProtocolApplication *app = (LcProtocolApplication *)
            lc_application_row_get_data(row);
        if (app->type == LC_PROTOCOL_APPLICATION_TYPE_THIRD) {
            gtk_widget_show(GTK_WIDGET(row));
        } else {
            gtk_widget_hide(GTK_WIDGET(row));
        }
    }
}
