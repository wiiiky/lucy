/*
 * lcinstalldialog.c
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


#include "lcinstalldialog.h"
#include "lcadb.h"
#include "lcutil.h"
#include "lcnotify.h"
#include <gtk/gtk.h>


struct _LcInstallDialogPrivate {
    GtkButton *ok_button;
    GtkButton *cancel_button;
    GtkGrid *grid;

    GtkLabel *name_label;
    GtkFileChooserButton *file_button;
};

#define lc_install_dialog_get_priv(self)    ((self)->priv)
#define lc_install_dialog_get_file_button(self) \
        lc_install_dialog_get_priv(self)->file_button
#define lc_install_dialog_get_ok_button(self) \
        lc_install_dialog_get_priv(self)->ok_button


static gpointer lc_install_dialog_parent_class = NULL;

#define LC_INSTALL_DIALOG_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_LC_INSTALL_DIALOG, LcInstallDialogPrivate))
enum {
    LC_INSTALL_DIALOG_DUMMY_PROPERTY
};
static void lc_install_dialog_finalize(GObject * obj);
static void on_dialog_response(GtkDialog * dialog, gint response_id,
                               gpointer user_data);

LcInstallDialog *lc_install_dialog_construct(GType object_type,
                                             GtkWindow * parent)
{
    LcInstallDialog *self = NULL;
    g_return_val_if_fail(parent != NULL, NULL);
    self = (LcInstallDialog *) g_object_new(object_type,
                                            "modal", FALSE,
                                            "transient-for", parent,
                                            "destroy-with-parent", TRUE,
                                            "title", "Install APP",
                                            "default-width", 320,
                                            "default-height", 240, NULL);
    g_signal_connect(G_OBJECT(self), "response",
                     G_CALLBACK(on_dialog_response), self);
    gtk_widget_show_all(GTK_WIDGET(self));
    return self;
}


LcInstallDialog *lc_install_dialog_new(GtkWindow * parent)
{
    return lc_install_dialog_construct(TYPE_LC_INSTALL_DIALOG, parent);
}


static void lc_install_dialog_class_init(LcInstallDialogClass * klass)
{
    lc_install_dialog_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(LcInstallDialogPrivate));
    G_OBJECT_CLASS(klass)->finalize = lc_install_dialog_finalize;
}

static void on_file_changed(GtkFileChooserButton * button,
                            gpointer user_data);

static void lc_install_dialog_instance_init(LcInstallDialog * self)
{
    self->priv = LC_INSTALL_DIALOG_GET_PRIVATE(self);

    LcInstallDialogPrivate *priv = self->priv;
    priv->ok_button = (GtkButton *) gtk_button_new_with_label("OK");
    g_object_ref_sink(priv->ok_button);
    priv->cancel_button =
        (GtkButton *) gtk_button_new_with_label("CANCEL");
    g_object_ref_sink(priv->cancel_button);

    priv->grid = (GtkGrid *) gtk_grid_new();
    g_object_ref_sink(priv->grid);
    gtk_container_set_border_width(GTK_CONTAINER(priv->grid), 5);
    gtk_box_pack_start(GTK_BOX
                       (gtk_dialog_get_content_area(GTK_DIALOG(self))),
                       GTK_WIDGET(priv->grid), TRUE, TRUE, 0);

    priv->file_button = (GtkFileChooserButton *)
        gtk_file_chooser_button_new("Choose an APK file",
                                    GTK_FILE_CHOOSER_ACTION_OPEN);
    g_object_ref_sink(priv->file_button);
    g_signal_connect(G_OBJECT(priv->file_button), "file-set",
                     G_CALLBACK(on_file_changed), self);
    GtkFileFilter *all_filter = gtk_file_filter_new();
    gtk_file_filter_set_name(all_filter, "All Files");
    gtk_file_filter_add_pattern(all_filter, "*");
    GtkFileFilter *apk_filter = gtk_file_filter_new();
    gtk_file_filter_set_name(apk_filter, "APK Files");
    gtk_file_filter_add_pattern(apk_filter, "*.apk");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(priv->file_button),
                                apk_filter);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(priv->file_button),
                                all_filter);
    gtk_grid_attach(priv->grid, GTK_WIDGET(priv->file_button), 0, 0, 1, 1);


    gtk_dialog_add_action_widget(GTK_DIALOG(self),
                                 GTK_WIDGET(priv->cancel_button),
                                 GTK_RESPONSE_CANCEL);
    gtk_dialog_add_action_widget(GTK_DIALOG(self),
                                 GTK_WIDGET(priv->ok_button),
                                 GTK_RESPONSE_OK);

    gtk_widget_set_sensitive(GTK_WIDGET(priv->ok_button), FALSE);
}


static void lc_install_dialog_finalize(GObject * obj)
{
    LcInstallDialog *self;
    self =
        G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_LC_INSTALL_DIALOG,
                                   LcInstallDialog);
    LcInstallDialogPrivate *priv = self->priv;
    g_object_unref(priv->ok_button);
    g_object_unref(priv->cancel_button);
    g_object_unref(priv->grid);
    g_object_unref(priv->file_button);
    G_OBJECT_CLASS(lc_install_dialog_parent_class)->finalize(obj);
}

static void on_file_changed(GtkFileChooserButton * button,
                            gpointer user_data)
{
    LcInstallDialog *self = (LcInstallDialog *) user_data;
    GtkButton *ok_button = lc_install_dialog_get_ok_button(self);
    gchar *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    gtk_widget_set_sensitive(GTK_WIDGET(ok_button), path != NULL);
    g_free(path);
}

static void on_install_app(GObject * source_object, GAsyncResult * result,
                           gpointer user_data)
{
    gint ret = lc_adb_install_app_finish(result);
    gchar *path = (gchar *) user_data;
    if (ret) {
        g_warning("Failed to install %s", path);
        lc_notify_show("Lucy",
                       "Failed to install APP!", CONNECT_FAIL_ICON);
    } else {
        g_message("Install %s successfully", path);
        lc_notify_show("Lucy", "Install APP successfully!",
                       CONNECT_SUCCESS_ICON);
    }
    g_free(path);
}

static void on_dialog_response(GtkDialog * dialog, gint response_id,
                               gpointer user_data)
{
    LcInstallDialog *self = (LcInstallDialog *) user_data;
    if (response_id == GTK_RESPONSE_OK) {
        GtkFileChooserButton *file_button =
            lc_install_dialog_get_file_button(self);
        gchar *path =
            gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_button));
        if (path) {
            lc_adb_install_app(path, on_install_app, path);
        }
    }
    gtk_widget_destroy(GTK_WIDGET(self));
}

GType lc_install_dialog_get_type(void)
{
    static volatile gsize lc_install_dialog_type_id__volatile = 0;
    if (g_once_init_enter(&lc_install_dialog_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(LcInstallDialogClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) lc_install_dialog_class_init,
            (GClassFinalizeFunc) NULL,
            NULL, sizeof(LcInstallDialog), 0,
            (GInstanceInitFunc) lc_install_dialog_instance_init, NULL
        };
        GType lc_install_dialog_type_id;
        lc_install_dialog_type_id =
            g_type_register_static(GTK_TYPE_DIALOG, "LcInstallDialog",
                                   &g_define_type_info, 0);
        g_once_init_leave(&lc_install_dialog_type_id__volatile,
                          lc_install_dialog_type_id);
    }
    return lc_install_dialog_type_id__volatile;
}
