/*
 * ui_aboutdialog.h
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



#ifndef __UI_ABOUTDIALOG_H__
#define __UI_ABOUTDIALOG_H__

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS
#define TYPE_UI_ABOUT_DIALOG (ui_about_dialog_get_type ())
#define UI_ABOUT_DIALOG(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_UI_ABOUT_DIALOG, UIAboutDialog))
#define UI_ABOUT_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_UI_ABOUT_DIALOG, UIAboutDialogClass))
#define IS_UI_ABOUT_DIALOG(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_UI_ABOUT_DIALOG))
#define IS_UI_ABOUT_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_UI_ABOUT_DIALOG))
#define UI_ABOUT_DIALOG_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_UI_ABOUT_DIALOG, UIAboutDialogClass))
typedef struct _UIAboutDialog UIAboutDialog;
typedef struct _UIAboutDialogClass UIAboutDialogClass;
typedef struct _UIAboutDialogPrivate UIAboutDialogPrivate;

struct _UIAboutDialog {
    GtkAboutDialog parent_instance;
    UIAboutDialogPrivate *priv;
};

struct _UIAboutDialogClass {
    GtkAboutDialogClass parent_class;
};

#define PROGRAM_NAME "Lucy"
#define LUCY_WEBSITE "https://github.com/wiiiky/lucy"
#define LUCY_COMMENTS "Android Manager for Linux"
#define LUCY_COPYRIGH "Copyright (c) Wiky L"
#define LUCY_VERSION "0.2"

GType ui_about_dialog_get_type(void) G_GNUC_CONST;
UIAboutDialog *ui_about_dialog_new(void);
UIAboutDialog *ui_about_dialog_construct(GType object_type);

#define ui_about_dialog_run(dialog)    gtk_dialog_run(GTK_DIALOG(dialog))
#define ui_about_dialog_destroy(dialog) gtk_widget_destroy(GTK_WIDGET(dialog))


G_END_DECLS
#endif
