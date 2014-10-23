/*
 * ui_smsrow.h
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
#ifndef __UI_SMSROW_H__
#define __UI_SMSROW_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS
#define TYPE_UI_SMS_ROW (ui_sms_row_get_type ())
#define UI_SMS_ROW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_UI_SMS_ROW, UISMSRow))
#define UI_SMS_ROW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_UI_SMS_ROW, UISMSRowClass))
#define IS_UI_SMS_ROW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_UI_SMS_ROW))
#define IS_UI_SMS_ROW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_UI_SMS_ROW))
#define UI_SMS_ROW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_UI_SMS_ROW, UISMSRowClass))
typedef struct _UISMSRow UISMSRow;
typedef struct _UISMSRowClass UISMSRowClass;
typedef struct _UISMSRowPrivate UISMSRowPrivate;

struct _UISMSRow {
    GtkEventBox parent_instance;
    UISMSRowPrivate *priv;
};

struct _UISMSRowClass {
    GtkEventBoxClass parent_class;
};
GType ui_sms_row_get_type(void) G_GNUC_CONST;
UISMSRow *ui_sms_row_new(GList * data);
UISMSRow *ui_sms_row_new_take(GList * data);
UISMSRow *ui_sms_row_construct(GType object_type, GList * data);
void ui_sms_row_set_address(UISMSRow * self, const gchar * address);
void ui_sms_row_set_preview(UISMSRow * self, const gchar * preview);
void ui_sms_row_set_date(UISMSRow * self, const gchar * date);
void ui_sms_row_set_full(UISMSRow * self, const gchar * address,
                         const gchar * preview, const gchar * date);

GList *ui_sms_row_get_data(UISMSRow * self);
void ui_sms_row_set_data(UISMSRow * self, GList * list);
void ui_sms_row_set_data_take(UISMSRow * self, GList * list);

gint ui_sms_row_get_thread_id(UISMSRow * self);

void ui_sms_row_highlight(UISMSRow * self);
void ui_sms_row_unhighlight(UISMSRow * self);

G_END_DECLS
#endif
