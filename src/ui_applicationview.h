/*
 * ui_applicationview.h
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

#ifndef __UI_APPLICATIONVIEW_H__
#define __UI_APPLICATIONVIEW_H__

#include <glib.h>
#include <gtk/gtk.h>
#include "lcprotocol.h"

G_BEGIN_DECLS
#define TYPE_UI_APPLICATION_VIEW (ui_application_view_get_type ())
#define UI_APPLICATION_VIEW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_UI_APPLICATION_VIEW, UIApplicationView))
#define UI_APPLICATION_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_UI_APPLICATION_VIEW, UIApplicationViewClass))
#define IS_UI_APPLICATION_VIEW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_UI_APPLICATION_VIEW))
#define IS_UI_APPLICATION_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_UI_APPLICATION_VIEW))
#define UI_APPLICATION_VIEW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_UI_APPLICATION_VIEW, UIApplicationViewClass))
typedef struct _UIApplicationView UIApplicationView;
typedef struct _UIApplicationViewClass UIApplicationViewClass;
typedef struct _UIApplicationViewPrivate UIApplicationViewPrivate;

struct _UIApplicationView {
    GtkStack parent_instance;
    UIApplicationViewPrivate *priv;
};

struct _UIApplicationViewClass {
    GtkStackClass parent_class;
};


GType ui_application_view_get_type(void) G_GNUC_CONST;
UIApplicationView *ui_application_view_new(void);
UIApplicationView *ui_application_view_construct(GType object_type);

void ui_application_view_append_row(UIApplicationView * self,
                                    LcProtocolApplication * info);
void ui_application_view_remove_row(UIApplicationView * self, gint row);
void ui_application_view_update(UIApplicationView * self, GList * list);

guint64 ui_application_view_get_update_time(UIApplicationView * self);


LcProtocolApplicationType
ui_application_view_get_current_type(UIApplicationView * self);
void ui_application_view_update_view(UIApplicationView * self);

G_END_DECLS
#endif
