/*
 * lcsmsbox.h 	widget to show a SMS message
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


#ifndef __LCSMSBOX_H__
#define __LCSMSBOX_H__

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS
#define TYPE_LC_SMS_BOX (lc_sms_box_get_type ())
#define LC_SMS_BOX(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_LC_SMS_BOX, LcSmsBox))
#define LC_SMS_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_LC_SMS_BOX, LcSmsBoxClass))
#define IS_LC_SMS_BOX(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_LC_SMS_BOX))
#define IS_LC_SMS_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_LC_SMS_BOX))
#define LC_SMS_BOX_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_LC_SMS_BOX, LcSmsBoxClass))
typedef struct _LcSmsBox LcSmsBox;
typedef struct _LcSmsBoxClass LcSmsBoxClass;
typedef struct _LcSmsBoxPrivate LcSmsBoxPrivate;

struct _LcSmsBox {
    GtkDrawingArea parent_instance;
    LcSmsBoxPrivate *priv;
};

struct _LcSmsBoxClass {
    GtkDrawingAreaClass parent_class;
};


GType lc_sms_box_get_type(void) G_GNUC_CONST;
LcSmsBox *lc_sms_box_new(void);
LcSmsBox *lc_sms_box_new_with_text(const gchar * text);
LcSmsBox *lc_sms_box_construct(GType object_type);

const gchar *lc_sms_box_get_text(LcSmsBox * self);
void lc_sms_box_set_text(LcSmsBox * self, const gchar * text);


G_END_DECLS
#endif
