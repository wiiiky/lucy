/*
 * lcsmsrow.h
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


#ifndef __LCSMSROW_H__
#define __LCSMSROW_H__

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS
#define TYPE_LC_SMS_ROW (lc_sms_row_get_type ())
#define LC_SMS_ROW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_LC_SMS_ROW, LcSMSRow))
#define LC_SMS_ROW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_LC_SMS_ROW, LcSMSRowClass))
#define IS_LC_SMS_ROW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_LC_SMS_ROW))
#define IS_LC_SMS_ROW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_LC_SMS_ROW))
#define LC_SMS_ROW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_LC_SMS_ROW, LcSMSRowClass))
typedef struct _LcSMSRow LcSMSRow;
typedef struct _LcSMSRowClass LcSMSRowClass;
typedef struct _LcSMSRowPrivate LcSMSRowPrivate;

struct _LcSMSRow {
    GtkEventBox parent_instance;
    LcSMSRowPrivate *priv;
};

struct _LcSMSRowClass {
    GtkEventBoxClass parent_class;
};


GType lc_sms_row_get_type(void) G_GNUC_CONST;
LcSMSRow *lc_sms_row_new(void);
LcSMSRow *lc_sms_row_construct(GType object_type);


G_END_DECLS
#endif
