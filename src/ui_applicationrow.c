/*
 * ui_applicationrow.c
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
#include "ui_applicationrow.h"
#include "lccommander.h"
#include "lcutil.h"

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

#define ICON_SIZE       (48)
#define NAME_LABEL_WIDTH (180)
#define VERSION_LABEL_WIDTH (200)
#define TYPE_LABEL_WIDTH (120)
#define UNINSTALL_BUTTON_WIDTH  (100)

struct _UIApplicationRowPrivate {
    GtkGrid *grid;
    GtkImage *icon_image;
    GtkLabel *name_label;
    GtkLabel *version_label;
    GtkLabel *type_label;
    GtkButton *uninstall_button;

    gpointer user_data;
    GDestroyNotify destroy;
};

#define UI_APPLICATION_ROW_ICON_SIZE	(48)


static gpointer ui_application_row_parent_class = NULL;

#define UI_APPLICATION_ROW_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_UI_APPLICATION_ROW, UIApplicationRowPrivate))
enum {
    UI_APPLICATION_ROW_DUMMY_PROPERTY
};
static void ui_application_row_finalize(GObject * obj);


UIApplicationRow *ui_application_row_construct(GType object_type)
{
    UIApplicationRow *self = NULL;
    self = (UIApplicationRow *) g_object_new(object_type, NULL);
    gtk_widget_show_all(GTK_WIDGET(self));
    return self;
}


UIApplicationRow *ui_application_row_new(void)
{
    return ui_application_row_construct(TYPE_UI_APPLICATION_ROW);
}


static void ui_application_row_class_init(UIApplicationRowClass * klass)
{
    ui_application_row_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(UIApplicationRowPrivate));
    G_OBJECT_CLASS(klass)->finalize = ui_application_row_finalize;
}


static void ui_application_row_instance_init(UIApplicationRow * self)
{
    self->priv = UI_APPLICATION_ROW_GET_PRIVATE(self);

    UIApplicationRowPrivate *priv = self->priv;

    priv->grid = (GtkGrid *) gtk_grid_new();
    gtk_grid_set_column_spacing(priv->grid, 8);
    gtk_container_set_border_width(GTK_CONTAINER(priv->grid), 1);
    g_object_ref_sink(priv->grid);
    /* ICON */
    priv->icon_image = (GtkImage *) gtk_image_new();
    g_object_ref_sink(priv->icon_image);
    /* NAME */
    priv->name_label = (GtkLabel *) gtk_label_new("");
    gtk_misc_set_alignment(GTK_MISC(priv->name_label), 0.0, 0.5);
    gtk_widget_set_margin_left(GTK_WIDGET(priv->name_label), 10);
    gtk_label_set_max_width_chars(priv->name_label, 20);
    gtk_widget_set_size_request(GTK_WIDGET(priv->name_label),
                                NAME_LABEL_WIDTH, -1);
    gtk_label_set_ellipsize(priv->name_label, PANGO_ELLIPSIZE_END);
    g_object_ref_sink(priv->name_label);
    /* VERSION */
    priv->version_label = (GtkLabel *) gtk_label_new("");
    gtk_misc_set_alignment(GTK_MISC(priv->version_label), 0.0, 0.5);
    gtk_label_set_max_width_chars(priv->version_label, 20);
    gtk_label_set_ellipsize(priv->version_label, PANGO_ELLIPSIZE_END);
    gtk_widget_set_margin_left(GTK_WIDGET(priv->version_label), 20);
    gtk_widget_set_size_request(GTK_WIDGET(priv->version_label),
                                VERSION_LABEL_WIDTH, -1);
    g_object_ref_sink(priv->version_label);
    /* TYPE */
    priv->type_label = (GtkLabel *) gtk_label_new("");
    gtk_widget_set_size_request(GTK_WIDGET(priv->type_label),
                                TYPE_LABEL_WIDTH, -1);
    gtk_widget_set_margin_left(GTK_WIDGET(priv->type_label), 15);
    g_object_ref_sink(priv->type_label);
    /* UNINSTALL */
    priv->uninstall_button =
        (GtkButton *) gtk_button_new_with_label("uninstall");
    gtk_widget_set_size_request(GTK_WIDGET(priv->uninstall_button),
                                UNINSTALL_BUTTON_WIDTH, -1);
    gtk_widget_set_margin_left(GTK_WIDGET(priv->uninstall_button), 10);
    g_object_ref_sink(priv->uninstall_button);

    gtk_container_add(GTK_CONTAINER(self), GTK_WIDGET(priv->grid));

    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->icon_image), 0, 0, 1, 1);
    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->name_label), 1, 0, 1, 1);
    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->version_label),
                    2, 0, 1, 1);
    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->type_label), 3, 0, 1, 1);
    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->uninstall_button), 4, 0,
                    1, 1);

    priv->user_data = NULL;
    priv->destroy = NULL;
}


static void ui_application_row_finalize(GObject * obj)
{
    UIApplicationRow *self;
    self =
        G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_UI_APPLICATION_ROW,
                                   UIApplicationRow);
    _g_object_unref0(self->priv->grid);
    _g_object_unref0(self->priv->icon_image);
    _g_object_unref0(self->priv->name_label);
    _g_object_unref0(self->priv->version_label);
    _g_object_unref0(self->priv->type_label);
    _g_object_unref0(self->priv->uninstall_button);
    if (self->priv->user_data && self->priv->destroy) {
        self->priv->destroy(self->priv->user_data);
    }
    G_OBJECT_CLASS(ui_application_row_parent_class)->finalize(obj);
}

static const gchar *get_readable_type(LcProtocolApplicationType type)
{
    if (type == LC_PROTOCOL_APPLICATION_TYPE_SYSTEM) {
        return "SYSTEM";
    } else if (type == LC_PROTOCOL_APPLICATION_TYPE_THIRD) {
        return "THIRD";
    }
    return "UNKNOWN";
}

GType ui_application_row_get_type(void)
{
    static volatile gsize ui_application_row_type_id__volatile = 0;
    if (g_once_init_enter(&ui_application_row_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(UIApplicationRowClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) ui_application_row_class_init,
            (GClassFinalizeFunc) NULL, NULL, sizeof(UIApplicationRow), 0,
            (GInstanceInitFunc) ui_application_row_instance_init, NULL
        };
        GType ui_application_row_type_id;
        ui_application_row_type_id =
            g_type_register_static(GTK_TYPE_EVENT_BOX, "UIApplicationRow",
                                   &g_define_type_info, 0);
        g_once_init_leave(&ui_application_row_type_id__volatile,
                          ui_application_row_type_id);
    }
    return ui_application_row_type_id__volatile;
}

static void
ui_appliation_row_update_uninstall_button_visible(UIApplicationRow * self,
                                                  LcProtocolApplicationType
                                                  type)
{
    GtkButton *uninstall_button = self->priv->uninstall_button;
    if (type == LC_PROTOCOL_APPLICATION_TYPE_SYSTEM) {
        gtk_widget_set_visible(GTK_WIDGET(uninstall_button), FALSE);
    } else {
        gtk_widget_set_visible(GTK_WIDGET(uninstall_button), TRUE);
    }
}

UIApplicationRow *ui_application_row_new_full(GdkPixbuf * icon,
                                              const gchar * name,
                                              const gchar * version,
                                              LcProtocolApplicationType
                                              type)
{
    UIApplicationRow *self = ui_application_row_new();
    GtkImage *icon_image = self->priv->icon_image;
    GtkLabel *name_label = self->priv->name_label;
    GtkLabel *version_label = self->priv->version_label;
    GtkLabel *type_label = self->priv->type_label;

    gtk_image_set_from_pixbuf(icon_image, icon);
    gtk_label_set_text(name_label, name);
    gtk_widget_set_tooltip_text(GTK_WIDGET(name_label), name);
    gtk_label_set_text(version_label, version);
    gtk_widget_set_tooltip_text(GTK_WIDGET(version_label), version);
    gtk_label_set_text(type_label, get_readable_type(type));

    ui_appliation_row_update_uninstall_button_visible(self, type);

    return self;
}

#define SAVE_DATA_KEY "save-data"
static void on_icon_saved(GObject * file, GAsyncResult * result,
                          gpointer user_data)
{
    UIApplicationRow *self = (UIApplicationRow *) user_data;
    LcProtocolApplication *info = self->priv->user_data;
    gchar *data = g_object_get_data(file, SAVE_DATA_KEY);
    g_free(data);
    if (g_file_replace_contents_finish(G_FILE(file), result, NULL, NULL)) {
        GdkPixbuf *pixbuf =
            lc_util_load_pixbuf_from_cache_with_size(info->package_name,
                                                     ICON_SIZE, ICON_SIZE);
        ui_application_row_set_icon(self, pixbuf);
        g_object_unref(pixbuf);
    } else {
        g_warning("Failed to save %s", info->package_name);
    }
}

static void on_command_icon(const gchar * cmd, GByteArray * array,
                            gpointer user_data)
{
    UIApplicationRow *self = (UIApplicationRow *) user_data;
    GBytes *bytes = lc_util_get_bytes_from_byte_array(array);
    if (bytes == NULL) {
        g_warning("%s failed! Connection Problem!", cmd);
        return;
    }
    if (lc_protocol_get_result_from_bytes(bytes) !=
        LC_PROTOCOL_RESULT_OKAY) {
        g_warning("%s failed! Protocol Problem!", cmd);
        g_bytes_unref(bytes);
        return;
    }
    LcProtocolApplication *info = self->priv->user_data;
    const gchar *path =
        lc_util_get_image_cache_path_by_name(info->package_name);
    GFile *file = g_file_new_for_path(path);
    gsize size;
    gchar *data = g_bytes_unref_to_data(bytes, &size);
    g_file_replace_contents_async(file, data + LC_PROTOCOL_HDR_LEN,
                                  size - LC_PROTOCOL_HDR_LEN, NULL, FALSE,
                                  G_FILE_CREATE_NONE, NULL, on_icon_saved,
                                  self);
    g_object_set_data(G_OBJECT(file), SAVE_DATA_KEY, data);

    g_object_unref(file);
}

UIApplicationRow *ui_application_row_new_with_data(const
                                                   LcProtocolApplication *
                                                   info)
{
    GdkPixbuf *pixbuf =
        lc_util_load_pixbuf_from_cache_with_size(info->package_name,
                                                 ICON_SIZE, ICON_SIZE);
    gboolean icon = FALSE;
    if (pixbuf == NULL) {
        icon = TRUE;
        pixbuf =
            lc_util_load_pixbuf_from_resource_with_size
            (APPLICATION_DEFAULT_ICON, ICON_SIZE, ICON_SIZE);
    }
    UIApplicationRow *self =
        ui_application_row_new_full(pixbuf, info->app_name, info->version,
                                    info->type);
    ui_application_row_set_data(self, lc_protocol_application_copy(info),
                                (GDestroyNotify)
                                lc_protocol_application_free);
    g_object_unref(pixbuf);
    if (icon) {
        lc_commander_send_command_async(lc_protocol_icon_command
                                        (info->package_name),
                                        on_command_icon, self);
    }
    return self;
}

void ui_application_row_update_data(UIApplicationRow * self,
                                    const LcProtocolApplication * info)
{
    ui_application_row_set_data(self, lc_protocol_application_copy(info),
                                (GDestroyNotify)
                                lc_protocol_application_free);
    GtkLabel *name_label = self->priv->name_label;
    GtkLabel *version_label = self->priv->version_label;
    GtkLabel *type_label = self->priv->type_label;
    gtk_label_set_label(name_label, info->app_name);
    gtk_widget_set_tooltip_text(GTK_WIDGET(name_label), info->app_name);
    gtk_label_set_label(version_label, info->version);
    gtk_widget_set_tooltip_text(GTK_WIDGET(version_label), info->version);
    gtk_label_set_label(type_label, get_readable_type(info->type));

    ui_appliation_row_update_uninstall_button_visible(self, info->type);
}

void ui_application_row_set_data(UIApplicationRow * self, gpointer data,
                                 GDestroyNotify notify)
{
    GDestroyNotify destroy = self->priv->destroy;
    gpointer pdata = self->priv->user_data;
    if (destroy && pdata) {
        destroy(pdata);
    }
    self->priv->user_data = data;
    self->priv->destroy = notify;
}

gpointer ui_application_row_get_data(UIApplicationRow * self)
{
    return self->priv->user_data;
}

void ui_application_row_set_icon(UIApplicationRow * self,
                                 GdkPixbuf * pixbuf)
{
    GtkImage *image = self->priv->icon_image;
    gtk_image_set_from_pixbuf(image, pixbuf);
}

static GtkCssProvider *highlight_provider = NULL;
void ui_application_row_highlight(UIApplicationRow * self)
{
    if (highlight_provider == NULL) {
        highlight_provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(highlight_provider,
                                        "GtkGrid{background-color:lightblue;}",
                                        -1, NULL);
    }
    GtkStyleContext *style =
        gtk_widget_get_style_context(GTK_WIDGET(self->priv->grid));
    gtk_style_context_add_provider(style,
                                   GTK_STYLE_PROVIDER(highlight_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void ui_application_row_unhighlight(UIApplicationRow * self)
{
    GtkStyleContext *style =
        gtk_widget_get_style_context(GTK_WIDGET(self->priv->grid));
    gtk_style_context_remove_provider(style,
                                      GTK_STYLE_PROVIDER
                                      (highlight_provider));
}

GtkButton *ui_application_row_get_uninstall_button(UIApplicationRow * self)
{
    return self->priv->uninstall_button;
}
