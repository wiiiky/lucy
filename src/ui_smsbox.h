/*
 * ui_smsbox.h
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


#ifndef __UI_SMSBOX_H__
#define __UI_SMSBOX_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS
#define TYPE_UI_SMS_BOX (ui_sms_box_get_type ())
#define UI_SMS_BOX(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_UI_SMS_BOX, UISMSBox))
#define UI_SMS_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_UI_SMS_BOX, UISMSBoxClass))
#define IS_UI_SMS_BOX(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_UI_SMS_BOX))
#define IS_UI_SMS_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_UI_SMS_BOX))
#define UI_SMS_BOX_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_UI_SMS_BOX, UISMSBoxClass))
typedef struct _UISMSBox UISMSBox;
typedef struct _UISMSBoxClass UISMSBoxClass;
typedef struct _UISMSBoxPrivate UISMSBoxPrivate;

struct _UISMSBox {
    GtkScrolledWindow parent_instance;
    UISMSBoxPrivate *priv;
};

struct _UISMSBoxClass {
    GtkScrolledWindowClass parent_class;
};

typedef enum {
    SMS_FONT_STYLE_NOMARL = PANGO_STYLE_NORMAL,
    SMS_FONT_STYLE_ITALIC = PANGO_STYLE_ITALIC,
    SMS_FONT_STYLE_OBLIQUE = PANGO_STYLE_OBLIQUE,
} SMSFontStyle;

typedef GdkRGBA SMSFontColor;


GType ui_sms_box_get_type(void) G_GNUC_CONST;
UISMSBox *ui_sms_box_new(GList * list); //this function will copy the list
UISMSBox *ui_sms_box_new_take(GList * list);    //just take the list
UISMSBox *ui_sms_box_new_reverse(GList * list);
UISMSBox *ui_sms_box_construct(GType object_type, GList * list);


void ui_sms_box_set_body_font_full(UISMSBox * self,
                                   const gchar * font_name,
                                   gint font_size,
                                   SMSFontStyle font_style);
void ui_sms_box_set_body_font_family(UISMSBox * self,
                                     const gchar * font_name);
void ui_sms_box_set_body_font_size(UISMSBox * self, gint font_size);
void ui_sms_box_set_body_font_style(UISMSBox * self,
                                    SMSFontStyle font_style);

void ui_sms_box_set_date_font_full(UISMSBox * self,
                                   const gchar * font_name,
                                   gint font_size,
                                   SMSFontStyle font_style);
void ui_sms_box_set_date_font_family(UISMSBox * self,
                                     const gchar * font_name);
void ui_sms_box_set_date_font_size(UISMSBox * self, gint font_size);
void ui_sms_box_set_date_font_style(UISMSBox * self,
                                    SMSFontStyle font_style);

void ui_sms_box_set_address_font_full(UISMSBox * self,
                                      const gchar * font_name,
                                      gint font_size,
                                      SMSFontStyle font_style);
void ui_sms_box_set_address_font_family(UISMSBox * self,
                                        const gchar * font_name);
void ui_sms_box_set_address_font_size(UISMSBox * self, gint font_size);
void ui_sms_box_set_address_font_style(UISMSBox * self,
                                       SMSFontStyle font_style);

void ui_sms_box_set_margin_full(UISMSBox * self,
                                gint margin_top,
                                gint margin_bottom,
                                gint margin_left,
                                gint margin_right,
                                gint margin_text,
                                gint margin_date,
                                gint margin_address, gint message_spacing);

void ui_sms_box_show_address(UISMSBox * self, gboolean show);

void ui_sms_box_set_data(UISMSBox * self, GList * list);
void ui_sms_box_set_data_reverse(UISMSBox * self, GList * list);
void ui_sms_box_set_data_take(UISMSBox * self, GList * list);

void ui_sms_box_reverse(UISMSBox * self);


G_END_DECLS
#endif
