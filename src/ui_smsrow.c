/*
 * ui_smsrow.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ui_smsrow.h"
#include "lcutil.h"

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

struct _UISMSRowPrivate {
    GtkGrid *grid;
    GtkImage *icon;
    GtkLabel *address;
    GtkLabel *date;
    GtkLabel *preview;
    GList *data;
};
static gpointer ui_sms_row_parent_class = NULL;
#define UI_SMS_ROW_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_UI_SMS_ROW, UISMSRowPrivate))

enum {
    UI_SMS_ROW_DUMMY_PROPERTY
};
static void ui_sms_row_finalize(GObject * obj);

UISMSRow *ui_sms_row_construct(GType object_type, GList * data)
{
    UISMSRow *self = NULL;
    self = (UISMSRow *) g_object_new(object_type, NULL);
    self->priv->data = data;
    if (data) {
        LcProtocolSMS *sms = (LcProtocolSMS *) data->data;
        gchar *date = lc_util_date_time_format(sms->time, "%m - %d");
        ui_sms_row_set_full(self, sms->address, sms->body, date);
        g_free(date);
    }
    return self;
}

UISMSRow *ui_sms_row_new(GList * data)
{
    GList *copy =
        g_list_copy_deep(data, (GCopyFunc) lc_protocol_sms_copy, NULL);
    return ui_sms_row_construct(TYPE_UI_SMS_ROW, copy);
}

UISMSRow *ui_sms_row_new_take(GList * data)
{
    return ui_sms_row_construct(TYPE_UI_SMS_ROW, data);
}

static void ui_sms_row_class_init(UISMSRowClass * klass)
{
    ui_sms_row_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(UISMSRowPrivate));
    G_OBJECT_CLASS(klass)->finalize = ui_sms_row_finalize;
}

static void ui_sms_row_instance_init(UISMSRow * self)
{
    self->priv = UI_SMS_ROW_GET_PRIVATE(self);
    UISMSRowPrivate *priv = self->priv;
    priv->grid = (GtkGrid *) gtk_grid_new();
    g_object_ref_sink(priv->grid);
    gtk_container_set_border_width(GTK_CONTAINER(priv->grid), 6);
    gtk_container_add(GTK_CONTAINER(self), GTK_WIDGET(priv->grid));

    priv->icon = (GtkImage *)
        gtk_image_new_from_file(lc_util_get_resource_by_name
                                (DEFAULT_AVATAR_ICON));
    g_object_ref_sink(priv->icon);
    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->icon), 0, 0, 2, 2);

    priv->address = (GtkLabel *) gtk_label_new("10086");
    gtk_label_set_markup(priv->address, "<span color='blue'>10086</span>");
    gtk_label_set_use_markup(priv->address, TRUE);
    gtk_label_set_single_line_mode(priv->address, TRUE);
    g_object_ref_sink(priv->address);
    gtk_widget_set_halign(GTK_WIDGET(priv->address), GTK_ALIGN_START);
    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->address), 2, 0, 1, 1);

    priv->date = (GtkLabel *) gtk_label_new("10-23");
    gtk_widget_set_margin_right(GTK_WIDGET(priv->date), 10);
    gtk_widget_set_halign(GTK_WIDGET(priv->date), GTK_ALIGN_END);
    g_object_ref_sink(priv->date);
    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->date), 3, 0, 1, 1);

    priv->preview = (GtkLabel *)
        gtk_label_new
        ("Linux是一套免费使用和自由传播的类Unix操作系统，是一个基于POSIX和UNIX的多用户");
    g_object_ref_sink(priv->preview);
	gtk_widget_set_hexpand(GTK_WIDGET(priv->preview),TRUE);
    gtk_widget_set_halign(GTK_WIDGET(priv->preview), GTK_ALIGN_START);
    gtk_label_set_ellipsize(priv->preview, PANGO_ELLIPSIZE_END);
    gtk_label_set_single_line_mode(priv->preview, TRUE);
    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->preview), 2, 1, 2, 1);

    priv->data = NULL;
}

static void ui_sms_row_finalize(GObject * obj)
{
    UISMSRow *self;
    self = G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_UI_SMS_ROW, UISMSRow);
    _g_object_unref0(self->priv->grid);
    _g_object_unref0(self->priv->icon);
    _g_object_unref0(self->priv->address);
    _g_object_unref0(self->priv->date);
    _g_object_unref0(self->priv->preview);
    g_list_free_full(self->priv->data,
                     (GDestroyNotify) lc_protocol_sms_free);
    G_OBJECT_CLASS(ui_sms_row_parent_class)->finalize(obj);
}

GType ui_sms_row_get_type(void)
{
    static volatile gsize ui_sms_row_type_id__volatile = 0;
    if (g_once_init_enter(&ui_sms_row_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(UISMSRowClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) ui_sms_row_class_init,
            (GClassFinalizeFunc) NULL, NULL,
            sizeof(UISMSRow), 0,
            (GInstanceInitFunc) ui_sms_row_instance_init, NULL
        };
        GType ui_sms_row_type_id;
        ui_sms_row_type_id =
            g_type_register_static(GTK_TYPE_EVENT_BOX, "UISMSRow",
                                   &g_define_type_info, 0);
        g_once_init_leave(&ui_sms_row_type_id__volatile,
                          ui_sms_row_type_id);
    }
    return ui_sms_row_type_id__volatile;
}

void ui_sms_row_set_address(UISMSRow * self, const gchar * address)
{
    GtkLabel *label = self->priv->address;
    gchar buf[1024];
    g_snprintf(buf, sizeof(buf), "<span color='blue'>%s</span>", address);
    gtk_label_set_markup(label, buf);
}

void ui_sms_row_set_preview(UISMSRow * self, const gchar * preview)
{
    GtkLabel *label = self->priv->preview;
    gtk_label_set_text(label, preview);
}

void ui_sms_row_set_date(UISMSRow * self, const gchar * date)
{
    GtkLabel *label = self->priv->date;
    gtk_label_set_text(label, date);
}

void ui_sms_row_set_full(UISMSRow * self,
                         const gchar * address,
                         const gchar * preview, const gchar * date)
{
    ui_sms_row_set_address(self, address);
    ui_sms_row_set_preview(self, preview);
    ui_sms_row_set_date(self, date);
}
