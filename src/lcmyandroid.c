/*
 * lcmyandroid.c
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
#include "lcmyandroid.h"
#include "lcutil.h"
#include <gtk/gtk.h>
#include <gtk-2.0/gtk/gtkbutton.h>

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

struct _LcMyAndroidPrivate {
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

static gpointer lc_my_android_parent_class = NULL;

#define LC_MY_ANDROID_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_LC_MY_ANDROID, LcMyAndroidPrivate))
enum {
    LC_MY_ANDROID_DUMMY_PROPERTY
};
static void lc_my_android_finalize(GObject * obj);


LcMyAndroid *lc_my_android_construct(GType object_type)
{
    LcMyAndroid *self = NULL;
    self = (LcMyAndroid *) g_object_new(object_type,
                                        "transition-duration", 1000,
                                        "transition-type",
                                        GTK_STACK_TRANSITION_TYPE_CROSSFADE,
                                        NULL);
    return self;
}


LcMyAndroid *lc_my_android_new(void)
{
    return lc_my_android_construct(TYPE_LC_MY_ANDROID);
}


static void lc_my_android_class_init(LcMyAndroidClass * klass)
{
    lc_my_android_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(LcMyAndroidPrivate));
    G_OBJECT_CLASS(klass)->finalize = lc_my_android_finalize;
}

static void lc_my_android_disconnect_init(LcMyAndroid * self);
static void lc_my_android_connect_init(LcMyAndroid * self);

static void lc_my_android_instance_init(LcMyAndroid * self)
{
    self->priv = LC_MY_ANDROID_GET_PRIVATE(self);

    lc_my_android_disconnect_init(self);
    lc_my_android_connect_init(self);

    gtk_stack_add_named(GTK_STACK(self),
                        GTK_WIDGET(self->priv->disconnected),
                        STACK_NAME_DISCONNECTED);
    gtk_stack_add_named(GTK_STACK(self), GTK_WIDGET(self->priv->connected),
                        STACK_NAME_CONNECTED);

    gtk_stack_set_visible_child_name(GTK_STACK(self),
                                     STACK_NAME_DISCONNECTED);
}


static void lc_my_android_disconnect_init(LcMyAndroid * self)
{
    GtkGrid *dc = (GtkGrid *) gtk_grid_new();

    gtk_widget_set_name(GTK_WIDGET(dc), "grid");

    GtkWidget *button = gtk_button_new_with_label(BUTTON_CONNECT_LABEL);
    gtk_widget_set_name(button, "connect");
    gtk_grid_attach(dc, button, 0, 1, 1, 1);

    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(button, 300);

    lc_util_load_css(GTK_WIDGET(dc), "css/disconnected.css");

    gtk_widget_show_all(GTK_WIDGET(dc));

    self->priv->disconnected = dc;
    g_object_ref_sink(self->priv->disconnected);
    self->priv->connectButton = GTK_BUTTON(button);
    g_object_ref_sink(self->priv->connectButton);
}

static void lc_my_android_connect_init(LcMyAndroid * self)
{
    GtkGrid *cn = (GtkGrid *) gtk_grid_new();
    gtk_grid_set_column_spacing(cn,15);
    
    GtkWidget *label=gtk_label_new("brand:");
    gtk_grid_attach(cn,label,0,0,1,1);
    label=gtk_label_new("model:");
    gtk_grid_attach(cn,label,0,1,1,1);
    label=gtk_label_new("number:");
    gtk_grid_attach(cn,label,0,2,1,1);

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

static void lc_my_android_finalize(GObject * obj)
{
    LcMyAndroid *self;
    self =
        G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_LC_MY_ANDROID, LcMyAndroid);
    _g_object_unref0(self->priv->disconnected);
    _g_object_unref0(self->priv->connected);
    _g_object_unref0(self->priv->connectButton);
    _g_object_unref0(self->priv->number);
    _g_object_unref0(self->priv->brand);
    _g_object_unref0(self->priv->model);
    G_OBJECT_CLASS(lc_my_android_parent_class)->finalize(obj);
}


GType lc_my_android_get_type(void)
{
    static volatile gsize lc_my_android_type_id__volatile = 0;
    if (g_once_init_enter(&lc_my_android_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(LcMyAndroidClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) lc_my_android_class_init,
            (GClassFinalizeFunc) NULL, NULL,
            sizeof(LcMyAndroid), 0,
            (GInstanceInitFunc) lc_my_android_instance_init, NULL
        };
        GType lc_my_android_type_id;
        lc_my_android_type_id =
            g_type_register_static(GTK_TYPE_STACK, "LcMyAndroid",
                                   &g_define_type_info, 0);
        g_once_init_leave(&lc_my_android_type_id__volatile,
                          lc_my_android_type_id);
    }
    return lc_my_android_type_id__volatile;
}


void lc_my_android_set_connect_callback(LcMyAndroid * self,
                                        GCallback callback,
                                        gpointer user_data)
{
    g_signal_connect(G_OBJECT(self->priv->connectButton), "clicked",
                     callback, user_data);
}

void lc_my_android_show_disconnect(LcMyAndroid * self)
{
    gtk_stack_set_visible_child_name(GTK_STACK(self),
                                     STACK_NAME_DISCONNECTED);
    gtk_button_set_label(self->priv->connectButton, BUTTON_CONNECT_LABEL);
    gtk_widget_set_sensitive(GTK_WIDGET(self->priv->connectButton), TRUE);
}

void lc_my_android_show_connecting(LcMyAndroid * self)
{
    //lc_my_android_set_connecting_timeout(self);
    gtk_widget_set_sensitive(GTK_WIDGET(self->priv->connectButton), FALSE);
    gtk_button_set_label(self->priv->connectButton,
                         BUTTON_CONNECTING_LABEL);
    gtk_stack_set_visible_child_name(GTK_STACK(self),
                                     STACK_NAME_DISCONNECTED);
}

void lc_my_android_show_connected(LcMyAndroid * self)
{
    gtk_stack_set_visible_child_name(GTK_STACK(self),
                                     STACK_NAME_CONNECTED);
}

void lc_my_android_show_connected_with_info(LcMyAndroid * self,
                                            LcProtocolPhone * phone)
{
    lc_my_android_show_connected(self);

    gtk_label_set_text(self->priv->brand, phone->brand);
    gtk_label_set_text(self->priv->model, phone->model);
    gtk_label_set_text(self->priv->number, phone->number);
}
