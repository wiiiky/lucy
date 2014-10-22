/*
 * ui_installdialog.h
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


#ifndef __UI_INSTALLDIALOG_H__
#define __UI_INSTALLDIALOG_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS
#define TYPE_UI_INSTALL_DIALOG (ui_install_dialog_get_type ())
#define UI_INSTALL_DIALOG(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_UI_INSTALL_DIALOG, UIInstallDialog))
#define UI_INSTALL_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_UI_INSTALL_DIALOG, UIInstallDialogClass))
#define IS_UI_INSTALL_DIALOG(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_UI_INSTALL_DIALOG))
#define IS_UI_INSTALL_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_UI_INSTALL_DIALOG))
#define UI_INSTALL_DIALOG_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_UI_INSTALL_DIALOG, UIInstallDialogClass))
typedef struct _UIInstallDialog UIInstallDialog;
typedef struct _UIInstallDialogClass UIInstallDialogClass;
typedef struct _UIInstallDialogPrivate UIInstallDialogPrivate;

struct _UIInstallDialog {
    GtkDialog parent_instance;
    UIInstallDialogPrivate *priv;
};

struct _UIInstallDialogClass {
    GtkDialogClass parent_class;
};


GType ui_install_dialog_get_type(void) G_GNUC_CONST;
UIInstallDialog *ui_install_dialog_new(GtkWindow * parent);
UIInstallDialog *ui_install_dialog_construct(GType object_type,
                                             GtkWindow * parent);

#define ui_install_dialog_run(self) gtk_dialog_run(GTK_DIALOG(self))

G_END_DECLS
#endif
