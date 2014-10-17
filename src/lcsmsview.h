/*
 * lcsmsview.h
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


#ifndef __LCSMSVIEW_H__
#define __LCSMSVIEW_H__

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS
#define TYPE_LC_SMS_VIEW (lc_sms_view_get_type ())
#define LC_SMS_VIEW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_LC_SMS_VIEW, LcSMSView))
#define LC_SMS_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_LC_SMS_VIEW, LcSMSViewClass))
#define IS_LC_SMS_VIEW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_LC_SMS_VIEW))
#define IS_LC_SMS_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_LC_SMS_VIEW))
#define LC_SMS_VIEW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_LC_SMS_VIEW, LcSMSViewClass))
typedef struct _LcSMSView LcSMSView;
typedef struct _LcSMSViewClass LcSMSViewClass;
typedef struct _LcSMSViewPrivate LcSMSViewPrivate;

struct _LcSMSView {
    GtkScrolledWindow parent_instance;
    LcSMSViewPrivate *priv;
};

struct _LcSMSViewClass {
    GtkScrolledWindowClass parent_class;
};

typedef enum {
    LC_SMS_FONT_STYLE_NOMARL = PANGO_STYLE_NORMAL,
    LC_SMS_FONT_STYLE_ITALIC = PANGO_STYLE_ITALIC,
    LC_SMS_FONT_STYLE_OBLIQUE = PANGO_STYLE_OBLIQUE,
} LcSMSFontStyle;

typedef GdkRGBA LcSMSFontColor;


GType lc_sms_view_get_type(void) G_GNUC_CONST;
LcSMSView *lc_sms_view_new(GList * list);
LcSMSView *lc_sms_view_construct(GType object_type, GList * list);


void lc_sms_view_set_body_font_full(LcSMSView * self,
                                    const gchar * font_name,
                                    gint font_size,
                                    LcSMSFontStyle font_style);
void lc_sms_view_set_body_font_family(LcSMSView * self,
                                      const gchar * font_name);
void lc_sms_view_set_body_font_size(LcSMSView * self, gint font_size);
void lc_sms_view_set_body_font_style(LcSMSView * self,
                                     LcSMSFontStyle font_style);

void lc_sms_view_set_date_font_full(LcSMSView * self,
                                    const gchar * font_name,
                                    gint font_size,
                                    LcSMSFontStyle font_style);
void lc_sms_view_set_date_font_family(LcSMSView * self,
                                      const gchar * font_name);
void lc_sms_view_set_date_font_size(LcSMSView * self, gint font_size);
void lc_sms_view_set_date_font_style(LcSMSView * self,
                                     LcSMSFontStyle font_style);

void lc_sms_view_set_address_font_full(LcSMSView * self,
                                       const gchar * font_name,
                                       gint font_size,
                                       LcSMSFontStyle font_style);
void lc_sms_view_set_address_font_family(LcSMSView * self,
                                         const gchar * font_name);
void lc_sms_view_set_address_font_size(LcSMSView * self, gint font_size);
void lc_sms_view_set_address_font_style(LcSMSView * self,
                                        LcSMSFontStyle font_style);

void lc_sms_view_set_margin_full(LcSMSView * self,
                                 gint margin_top,
                                 gint margin_bottom,
                                 gint margin_left,
                                 gint margin_right,
                                 gint margin_text,
                                 gint margin_date,
                                 gint margin_address,
                                 gint message_spacing);


G_END_DECLS
#endif
