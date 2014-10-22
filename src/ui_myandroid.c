/*
 * ui_myandroid.c
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


#include <glib.h>
#include <glib-object.h>
#include "ui_myandroid.h"
#include "lcutil.h"
#include <gtk/gtk.h>

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

struct _UIMyAndroidPrivate {
    GtkGrid *disconnected;
    GtkGrid *connected;
    GtkButton *connectButton;

    /* connected widget */
    GtkLabel *brand;
    GtkLabel *model;
    GtkLabel *number;
};

#define STACK_NAME_DISCONNECTED "disconnected"
#define STACK_NAME_CONNECTED	"connected"

#define BUTTON_CONNECT_LABEL	"Connect"
#define BUTTON_CONNECTING_LABEL	"Connecting"

static gpointer ui_my_android_parent_class = NULL;

#define UI_MY_ANDROID_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_UI_MY_ANDROID, UIMyAndroidPrivate))
enum {
    UI_MY_ANDROID_DUMMY_PROPERTY
};
static void ui_my_android_finalize(GObject * obj);


UIMyAndroid *ui_my_android_construct(GType object_type)
{
    UIMyAndroid *self = NULL;
    self = (UIMyAndroid *) g_object_new(object_type,
                                        "transition-duration", 1000,
                                        "transition-type",
                                        GTK_STACK_TRANSITION_TYPE_CROSSFADE,
                                        NULL);

    lc_util_load_css(GTK_WIDGET(self), MY_ANDROID_CSS_FILE);
    return self;
}


UIMyAndroid *ui_my_android_new(void)
{
    return ui_my_android_construct(TYPE_UI_MY_ANDROID);
}


static void ui_my_android_class_init(UIMyAndroidClass * klass)
{
    ui_my_android_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(UIMyAndroidPrivate));
    G_OBJECT_CLASS(klass)->finalize = ui_my_android_finalize;
}

static void ui_my_android_disconnect_init(UIMyAndroid * self);
static void ui_my_android_connect_init(UIMyAndroid * self);

static void ui_my_android_instance_init(UIMyAndroid * self)
{
    self->priv = UI_MY_ANDROID_GET_PRIVATE(self);

    ui_my_android_disconnect_init(self);
    ui_my_android_connect_init(self);

    gtk_stack_add_named(GTK_STACK(self),
                        GTK_WIDGET(self->priv->disconnected),
                        STACK_NAME_DISCONNECTED);
    gtk_stack_add_named(GTK_STACK(self), GTK_WIDGET(self->priv->connected),
                        STACK_NAME_CONNECTED);

    gtk_stack_set_visible_child_name(GTK_STACK(self),
                                     STACK_NAME_DISCONNECTED);
}


static void ui_my_android_disconnect_init(UIMyAndroid * self)
{
    GtkGrid *dc = (GtkGrid *) gtk_grid_new();

    gtk_widget_set_name(GTK_WIDGET(dc), "dc_grid");

    GtkWidget *button = gtk_button_new_with_label(BUTTON_CONNECT_LABEL);
    gtk_widget_set_name(button, "connect");
    gtk_grid_attach(dc, button, 0, 1, 1, 1);

    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(button, 300);

    gtk_widget_show_all(GTK_WIDGET(dc));

    self->priv->disconnected = dc;
    g_object_ref_sink(self->priv->disconnected);
    self->priv->connectButton = GTK_BUTTON(button);
    g_object_ref_sink(self->priv->connectButton);
}

static void ui_my_android_connect_init(UIMyAndroid * self)
{
    GtkGrid *cn = (GtkGrid *) gtk_grid_new();
    gtk_widget_set_name(GTK_WIDGET(cn), "c_grid");
    gtk_grid_set_column_spacing(cn, 15);

    GtkWidget *label = gtk_label_new("brand:");
    gtk_grid_attach(cn, label, 0, 0, 1, 1);
    label = gtk_label_new("model:");
    gtk_grid_attach(cn, label, 0, 1, 1, 1);
    label = gtk_label_new("number:");
    gtk_grid_attach(cn, label, 0, 2, 1, 1);

    GtkWidget *brand = gtk_label_new("");
    GtkWidget *model = gtk_label_new("");
    GtkWidget *number = gtk_label_new("");
    gtk_grid_attach(cn, brand, 1, 0, 1, 1);
    gtk_grid_attach(cn, model, 1, 1, 1, 1);
    gtk_grid_attach(cn, number, 1, 2, 1, 1);

    gtk_widget_show_all(GTK_WIDGET(cn));

    self->priv->connected = cn;
    self->priv->brand = (GtkLabel *) brand;
    self->priv->model = (GtkLabel *) model;
    self->priv->number = (GtkLabel *) number;
    g_object_ref_sink(self->priv->brand);
    g_object_ref_sink(self->priv->model);
    g_object_ref_sink(self->priv->number);
    g_object_ref_sink(self->priv->connected);
}

static void ui_my_android_finalize(GObject * obj)
{
    UIMyAndroid *self;
    self =
        G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_UI_MY_ANDROID, UIMyAndroid);
    _g_object_unref0(self->priv->disconnected);
    _g_object_unref0(self->priv->connected);
    _g_object_unref0(self->priv->connectButton);
    _g_object_unref0(self->priv->number);
    _g_object_unref0(self->priv->brand);
    _g_object_unref0(self->priv->model);
    G_OBJECT_CLASS(ui_my_android_parent_class)->finalize(obj);
}


GType ui_my_android_get_type(void)
{
    static volatile gsize ui_my_android_type_id__volatile = 0;
    if (g_once_init_enter(&ui_my_android_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(UIMyAndroidClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) ui_my_android_class_init,
            (GClassFinalizeFunc) NULL, NULL,
            sizeof(UIMyAndroid), 0,
            (GInstanceInitFunc) ui_my_android_instance_init, NULL
        };
        GType lc_my_android_type_id;
        lc_my_android_type_id =
            g_type_register_static(GTK_TYPE_STACK, "UIMyAndroid",
                                   &g_define_type_info, 0);
        g_once_init_leave(&ui_my_android_type_id__volatile,
                          lc_my_android_type_id);
    }
    return ui_my_android_type_id__volatile;
}


void ui_my_android_set_connect_callback(UIMyAndroid * self,
                                        GCallback callback,
                                        gpointer user_data)
{
    g_signal_connect(G_OBJECT(self->priv->connectButton), "clicked",
                     callback, user_data);
}

void ui_my_android_show_disconnect(UIMyAndroid * self)
{
    gtk_stack_set_visible_child_name(GTK_STACK(self),
                                     STACK_NAME_DISCONNECTED);
    gtk_button_set_label(self->priv->connectButton, BUTTON_CONNECT_LABEL);
    gtk_widget_set_sensitive(GTK_WIDGET(self->priv->connectButton), TRUE);
}

void ui_my_android_show_connecting(UIMyAndroid * self)
{
    //lc_my_android_set_connecting_timeout(self);
    gtk_widget_set_sensitive(GTK_WIDGET(self->priv->connectButton), FALSE);
    gtk_button_set_label(self->priv->connectButton,
                         BUTTON_CONNECTING_LABEL);
    gtk_stack_set_visible_child_name(GTK_STACK(self),
                                     STACK_NAME_DISCONNECTED);
}

void ui_my_android_show_connected(UIMyAndroid * self)
{
    gtk_stack_set_visible_child_name(GTK_STACK(self),
                                     STACK_NAME_CONNECTED);
}

void ui_my_android_show_connected_with_info(UIMyAndroid * self,
                                            LcProtocolPhone * phone)
{
    ui_my_android_show_connected(self);

    gtk_label_set_text(self->priv->brand, phone->brand);
    gtk_label_set_text(self->priv->model, phone->model);
    gtk_label_set_text(self->priv->number, phone->number);
}
