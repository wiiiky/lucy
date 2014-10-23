/*
 * ui_smsview.h
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


#ifndef __UI_SMSVIEW_H__
#define __UI_SMSVIEW_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS
#define UI_SMS_TYPE_VIEW (ui_sms_view_get_type ())
#define UI_SMS_VIEW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), UI_SMS_TYPE_VIEW, UISMSView))
#define UI_SMS_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), UI_SMS_TYPE_VIEW, UISMSViewClass))
#define UI_SMS_IS_VIEW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), UI_SMS_TYPE_VIEW))
#define UI_SMS_IS_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), UI_SMS_TYPE_VIEW))
#define UI_SMS_VIEW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), UI_SMS_TYPE_VIEW, UISMSViewClass))
typedef struct _UISMSView UISMSView;
typedef struct _UISMSViewClass UISMSViewClass;
typedef struct _UISMSViewPrivate UISMSViewPrivate;

struct _UISMSView {
    GtkStack parent_instance;
    UISMSViewPrivate *priv;
};

struct _UISMSViewClass {
    GtkStackClass parent_class;
};


GType ui_sms_view_get_type(void) G_GNUC_CONST;
UISMSView *ui_sms_view_new(GList * list);   /* will take the list */
UISMSView *ui_sms_view_construct(GType object_type, GList * list);

void ui_sms_view_update(UISMSView * self, GList * list);
void ui_sms_view_append_row(UISMSView * self, GList * list);
void ui_sms_view_append_row_take(UISMSView * self, GList * list);


G_END_DECLS
#endif
