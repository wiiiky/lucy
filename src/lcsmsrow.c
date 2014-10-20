/*
 * lcsmsrow.c
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
#include "lcsmsrow.h"
#include <gtk/gtk.h>

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

struct _LcSMSRowPrivate {
    GtkGrid *grid;

    GtkImage *icon;
    GtkLabel *address;
    GtkLabel *date;
    GtkLabel *preview;
};


static gpointer lc_sms_row_parent_class = NULL;

#define LC_SMS_ROW_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_LC_SMS_ROW, LcSMSRowPrivate))
enum {
    LC_SMS_ROW_DUMMY_PROPERTY
};
static void lc_sms_row_finalize(GObject * obj);


LcSMSRow *lc_sms_row_construct(GType object_type)
{
    LcSMSRow *self = NULL;
    self = (LcSMSRow *) g_object_new(object_type, NULL);
    return self;
}


LcSMSRow *lc_sms_row_new(void)
{
    return lc_sms_row_construct(TYPE_LC_SMS_ROW);
}


static void lc_sms_row_class_init(LcSMSRowClass * klass)
{
    lc_sms_row_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(LcSMSRowPrivate));
    G_OBJECT_CLASS(klass)->finalize = lc_sms_row_finalize;
}


static void lc_sms_row_instance_init(LcSMSRow * self)
{
    self->priv = LC_SMS_ROW_GET_PRIVATE(self);

    LcSMSRowPrivate *priv = self->priv;
    priv->grid = (GtkGrid *) gtk_grid_new();
    g_object_ref_sink(priv->grid);
    gtk_container_set_border_width(GTK_CONTAINER(priv->grid), 6);
    gtk_container_add(GTK_CONTAINER(self), GTK_WIDGET(priv->grid));

    priv->icon = (GtkImage *) gtk_image_new();
    g_object_ref_sink(priv->icon);
    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->icon), 0, 0, 2, 2);

    priv->address = (GtkLabel *) gtk_label_new("10086");
    g_object_ref_sink(priv->address);
    gtk_widget_set_halign(GTK_WIDGET(priv->address), GTK_ALIGN_START);
    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->address), 2, 0, 1, 1);

    priv->date = (GtkLabel *) gtk_label_new("10-23");
    gtk_widget_set_halign(GTK_WIDGET(priv->date), GTK_ALIGN_END);
    g_object_ref_sink(priv->date);
    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->date), 3, 0, 1, 1);

    priv->preview =
        (GtkLabel *)
        gtk_label_new
        ("Linux是一套免费使用和自由传播的类Unix操作系统，是一个基于POSIX和UNIX的多用户");
    g_object_ref_sink(priv->preview);
    gtk_widget_set_halign(GTK_WIDGET(priv->preview), GTK_ALIGN_START);
    gtk_label_set_ellipsize(priv->preview, PANGO_ELLIPSIZE_END);
    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->preview), 2, 1, 2, 1);
}


static void lc_sms_row_finalize(GObject * obj)
{
    LcSMSRow *self;
    self = G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_LC_SMS_ROW, LcSMSRow);
    _g_object_unref0(self->priv->grid);
    _g_object_unref0(self->priv->icon);
    _g_object_unref0(self->priv->address);
    _g_object_unref0(self->priv->date);
    _g_object_unref0(self->priv->preview);
    G_OBJECT_CLASS(lc_sms_row_parent_class)->finalize(obj);
}


GType lc_sms_row_get_type(void)
{
    static volatile gsize lc_sms_row_type_id__volatile = 0;
    if (g_once_init_enter(&lc_sms_row_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(LcSMSRowClass), (GBaseInitFunc) NULL,
(GBaseFinalizeFunc) NULL, (GClassInitFunc) lc_sms_row_class_init, (GClassFinalizeFunc) NULL, NULL,
sizeof(LcSMSRow), 0, (GInstanceInitFunc) lc_sms_row_instance_init, NULL };
        GType lc_sms_row_type_id;
        lc_sms_row_type_id =
            g_type_register_static(GTK_TYPE_EVENT_BOX, "LcSMSRow",
                                   &g_define_type_info, 0);
        g_once_init_leave(&lc_sms_row_type_id__volatile,
                          lc_sms_row_type_id);
    }
    return lc_sms_row_type_id__volatile;
}
