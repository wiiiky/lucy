/*
 * ui_aboutdialog.c
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
#include "ui_aboutdialog.h"

const static gchar *LUCY_AUTHORS[] = {
    "Wiky L (wiiiky@yeah.net)",
    NULL
};


static gpointer ui_about_dialog_parent_class = NULL;

enum {
    UI_ABOUT_DIALOG_DUMMY_PROPERTY
};


UIAboutDialog *ui_about_dialog_construct(GType object_type)
{
    UIAboutDialog *self = NULL;
    self = (UIAboutDialog *) g_object_new(object_type, NULL);
    return self;
}


UIAboutDialog *ui_about_dialog_new(void)
{
    return ui_about_dialog_construct(TYPE_UI_ABOUT_DIALOG);
}


static void ui_about_dialog_class_init(UIAboutDialogClass * klass)
{
    ui_about_dialog_parent_class = g_type_class_peek_parent(klass);
}


static void ui_about_dialog_instance_init(UIAboutDialog * self)
{
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(self),
                                      PROGRAM_NAME);
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(self), LUCY_VERSION);
    gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(self),
                                      GTK_LICENSE_GPL_3_0);
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(self), LUCY_COPYRIGH);
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(self), LUCY_COMMENTS);
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(self), LUCY_WEBSITE);
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(self), LUCY_AUTHORS);
}


GType ui_about_dialog_get_type(void)
{
    static volatile gsize ui_about_dialog_type_id__volatile = 0;
    if (g_once_init_enter(&ui_about_dialog_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(UIAboutDialogClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) ui_about_dialog_class_init,
            (GClassFinalizeFunc) NULL,
            NULL, sizeof(UIAboutDialog), 0,
            (GInstanceInitFunc) ui_about_dialog_instance_init, NULL
        };
        GType lc_about_dialog_type_id;
        lc_about_dialog_type_id =
            g_type_register_static(GTK_TYPE_ABOUT_DIALOG, "UIAboutDialog",
                                   &g_define_type_info, 0);
        g_once_init_leave(&ui_about_dialog_type_id__volatile,
                          lc_about_dialog_type_id);
    }
    return ui_about_dialog_type_id__volatile;
}
