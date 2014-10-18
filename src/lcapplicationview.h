/*
 * lcapplicationview.h
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

#ifndef __LCAPPLICATIONVIEW_H__
#define __LCAPPLICATIONVIEW_H__

#include <glib.h>
#include <gtk/gtk.h>
#include "lcprotocol.h"

G_BEGIN_DECLS
#define TYPE_LC_APPLICATION_VIEW (lc_application_view_get_type ())
#define LC_APPLICATION_VIEW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_LC_APPLICATION_VIEW, LcApplicationView))
#define LC_APPLICATION_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_LC_APPLICATION_VIEW, LcApplicationViewClass))
#define IS_LC_APPLICATION_VIEW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_LC_APPLICATION_VIEW))
#define IS_LC_APPLICATION_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_LC_APPLICATION_VIEW))
#define LC_APPLICATION_VIEW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_LC_APPLICATION_VIEW, LcApplicationViewClass))
typedef struct _LcApplicationView LcApplicationView;
typedef struct _LcApplicationViewClass LcApplicationViewClass;
typedef struct _LcApplicationViewPrivate LcApplicationViewPrivate;

struct _LcApplicationView {
    GtkStack parent_instance;
    LcApplicationViewPrivate *priv;
};

struct _LcApplicationViewClass {
    GtkStackClass parent_class;
};


GType lc_application_view_get_type(void) G_GNUC_CONST;
LcApplicationView *lc_application_view_new(void);
LcApplicationView *lc_application_view_construct(GType object_type);

void lc_application_view_append_row(LcApplicationView * self,
                                    LcProtocolApplication * info);
void lc_application_view_remove_row(LcApplicationView * self, gint row);
void lc_application_view_update(LcApplicationView * self, GList * list);

guint64 lc_application_view_get_update_time(LcApplicationView * self);

void lc_application_view_set_loading(LcApplicationView * self,
                                     gboolean loading);
gboolean lc_application_view_is_loading(LcApplicationView * self);

LcProtocolApplicationType
lc_application_view_get_current_type(LcApplicationView * self);
void lc_application_view_update_view(LcApplicationView * self);

G_END_DECLS
#endif
