/*
 * lcsmsview.c
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


#include "lcsmsview.h"
#include "lcprotocol.h"
#include <gtk/gtk.h>
#include <math.h>

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

struct _LcSMSViewPrivate {
    GtkDrawingArea *draw_area;

    gchar *body_font;           /* the font name of content */
    gint body_size;             /* the font size of content */
    LcSMSFontStyle body_style;  /* the font style of content */
    LcSMSFontColor body_color;  /* the color of content */

    gchar *date_font;           /* the font name of date */
    gint date_size;             /* the font size of date */
    LcSMSFontStyle date_style;  /* the font style of date */
    LcSMSFontColor date_color;  /* the color of date */

    gchar *address_font;        /* the font name of address */
    gint address_size;          /* the font size of address */
    LcSMSFontStyle address_style;   /* the font style of address */
    LcSMSFontColor address_color;   /* the color of address */

    gint margin_top;
    gint margin_bottom;
    gint margin_left;
    gint margin_right;
    gint margin_text;
    gint margin_date;
    gint margin_address;
    gint message_spacing;
    gint margin_inbox;
    gint margin_sent;

    gboolean show_address;

    GList *list;
};


static gpointer lc_sms_view_parent_class = NULL;

#define LC_SMS_VIEW_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_LC_SMS_VIEW, LcSMSViewPrivate))
enum {
    LC_SMS_VIEW_DUMMY_PROPERTY
};
static void lc_sms_view_finalize(GObject * obj);
static gboolean on_draw(GtkWidget * widget, cairo_t * cr, gpointer data);

/* draw a dialog box */
static void on_draw_box(cairo_t * cr, gint x, gint y, gint width,
                        gint height, gboolean left);


LcSMSView *lc_sms_view_construct(GType object_type, GList * list)
{
    LcSMSView *self = NULL;
    self = (LcSMSView *) g_object_new(object_type, NULL);
    self->priv->list = list;

    gtk_container_add(GTK_CONTAINER(self),
                      GTK_WIDGET(self->priv->draw_area));
    return self;
}


LcSMSView *lc_sms_view_new(GList * list)
{
    GList *copy =
        g_list_copy_deep(list, (GCopyFunc) lc_protocol_sms_copy, NULL);
    return lc_sms_view_construct(TYPE_LC_SMS_VIEW, copy);
}

LcSMSView *lc_sms_view_new_take(GList * list)
{
    return lc_sms_view_construct(TYPE_LC_SMS_VIEW, list);
}


static void lc_sms_view_class_init(LcSMSViewClass * klass)
{
    lc_sms_view_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(LcSMSViewPrivate));
    G_OBJECT_CLASS(klass)->finalize = lc_sms_view_finalize;
}


static void lc_sms_view_instance_init(LcSMSView * self)
{
    self->priv = LC_SMS_VIEW_GET_PRIVATE(self);

    LcSMSViewPrivate *priv = self->priv;

    priv->draw_area = (GtkDrawingArea *) gtk_drawing_area_new();
    g_signal_connect(G_OBJECT(priv->draw_area), "draw",
                     G_CALLBACK(on_draw), self);
    g_object_ref_sink(priv->draw_area);

    priv->list = NULL;
    priv->body_font = g_strdup("ubuntu mono");
    priv->body_size = 12;
    priv->body_style = LC_SMS_FONT_STYLE_NOMARL;
    priv->body_color.red = 0;
    priv->body_color.green = 0;
    priv->body_color.blue = 0;
    priv->body_color.alpha = 1;

    priv->date_font = g_strdup("consolas");
    priv->date_size = 10;
    priv->date_style = LC_SMS_FONT_STYLE_ITALIC;
    priv->date_color.red = 0.1;
    priv->date_color.green = 0.1;
    priv->date_color.blue = 0.1;
    priv->date_color.alpha = 0.6;

    priv->address_font = g_strdup("consolas");
    priv->address_size = 10;
    priv->address_style = LC_SMS_FONT_STYLE_NOMARL;
    priv->address_color.red = 0.1;
    priv->address_color.green = 0.1;
    priv->address_color.blue = 1.1;
    priv->address_color.alpha = 0.8;

    priv->margin_top = 20;
    priv->margin_bottom = 30;
    priv->margin_left = 18;
    priv->margin_right = 18;
    priv->margin_text = 8;
    priv->margin_date = 5;
    priv->margin_address = 8;
    priv->message_spacing = 32;
    priv->margin_inbox = 120;
    priv->margin_sent = 120;

    priv->show_address = TRUE;
}


static void lc_sms_view_finalize(GObject * obj)
{
    LcSMSView *self;
    self = G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_LC_SMS_VIEW, LcSMSView);
    _g_object_unref0(self->priv->draw_area);
    g_list_free_full(self->priv->list,
                     (GDestroyNotify) lc_protocol_sms_free);
    g_free(self->priv->body_font);
    g_free(self->priv->date_font);
    g_free(self->priv->address_font);
    G_OBJECT_CLASS(lc_sms_view_parent_class)->finalize(obj);
}


GType lc_sms_view_get_type(void)
{
    static volatile gsize lc_sms_view_type_id__volatile = 0;
    if (g_once_init_enter(&lc_sms_view_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(LcSMSViewClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) lc_sms_view_class_init,
            (GClassFinalizeFunc) NULL, NULL,
            sizeof(LcSMSView), 0,
            (GInstanceInitFunc) lc_sms_view_instance_init, NULL
        };
        GType lc_sms_view_type_id;
        lc_sms_view_type_id =
            g_type_register_static(GTK_TYPE_SCROLLED_WINDOW, "LcSMSView",
                                   &g_define_type_info, 0);
        g_once_init_leave(&lc_sms_view_type_id__volatile,
                          lc_sms_view_type_id);
    }
    return lc_sms_view_type_id__volatile;
}


static gboolean on_draw(GtkWidget * widget, cairo_t * cr, gpointer data)
{
    LcSMSView *self = (LcSMSView *) data;
    GList *list = self->priv->list;
    gint _width = gtk_widget_get_allocated_width(widget);
    if (_width <= 100) {
        return FALSE;
    }
    gint margin_left = self->priv->margin_left;
    gint margin_right = self->priv->margin_right;
    gint margin_top = self->priv->margin_top;
    gint margin_bottom = self->priv->margin_bottom;
    gint margin_text = self->priv->margin_text;
    gint margin_date = self->priv->margin_date;
    gint margin_address = self->priv->margin_address;
    gint message_spacing = self->priv->message_spacing;
    gint margin_inbox = self->priv->margin_inbox;
    gint margin_sent = self->priv->margin_sent;

    gdouble body_r = self->priv->body_color.red;
    gdouble body_g = self->priv->body_color.green;
    gdouble body_b = self->priv->body_color.blue;
    gdouble body_a = self->priv->body_color.alpha;
    gdouble date_r = self->priv->date_color.red;
    gdouble date_g = self->priv->date_color.green;
    gdouble date_b = self->priv->date_color.blue;
    gdouble date_a = self->priv->date_color.alpha;
    gdouble address_r = self->priv->address_color.red;
    gdouble address_g = self->priv->address_color.green;
    gdouble address_b = self->priv->address_color.blue;
    gdouble address_a = self->priv->address_color.alpha;

    gboolean show_address = self->priv->show_address;

    PangoFontDescription *body_font = pango_font_description_new();
    pango_font_description_set_size(body_font,
                                    self->priv->body_size * PANGO_SCALE);
    pango_font_description_set_family(body_font, self->priv->body_font);
    pango_font_description_set_style(body_font, self->priv->body_style);

    PangoLayout *body_layout = pango_cairo_create_layout(cr);
    pango_layout_set_font_description(body_layout, body_font);

    PangoFontDescription *date_font = pango_font_description_new();
    pango_font_description_set_size(date_font,
                                    self->priv->date_size * PANGO_SCALE);
    pango_font_description_set_family(date_font, self->priv->date_font);
    pango_font_description_set_style(date_font, self->priv->date_style);

    PangoLayout *date_layout = pango_cairo_create_layout(cr);
    pango_layout_set_font_description(date_layout, date_font);

    PangoFontDescription *address_font = pango_font_description_new();
    pango_font_description_set_size(address_font,
                                    self->priv->address_size *
                                    PANGO_SCALE);
    pango_font_description_set_family(address_font,
                                      self->priv->address_font);
    pango_font_description_set_style(address_font,
                                     self->priv->address_style);

    PangoLayout *address_layout = pango_cairo_create_layout(cr);
    pango_layout_set_font_description(address_layout, address_font);

    gint height = margin_top;
    while (list) {
        LcProtocolSMS *sms = (LcProtocolSMS *) list->data;
        gint w1, h1, w2, h2, w3, h3;

        gint width = 0;
        gint offset = 0;
        if (sms->type == LC_PROTOCOL_SMS_TYPE_INBOX) {
            width = _width - margin_inbox;
        } else {
            width = _width - margin_sent;
            offset = margin_sent;
        }
        gint text_width =
            (width - margin_left - margin_right - 2 * margin_text);
        gint pango_width = text_width * PANGO_SCALE;

        pango_layout_set_width(body_layout, pango_width);
        pango_layout_set_width(date_layout, pango_width);
        pango_layout_set_width(address_layout, pango_width);

        if (offset == 0) {
            /* left */
            // draw address
            if (show_address) {
                cairo_set_source_rgba(cr, address_r, address_g, address_b,
                                      address_a);
                pango_layout_set_text(address_layout, sms->address, -1);
                pango_layout_get_pixel_size(address_layout, &w3, &h3);
                cairo_move_to(cr, margin_left, height);
                pango_cairo_show_layout(cr, address_layout);
            } else {
                w3 = -margin_address;
            }

            // draw date
            cairo_set_source_rgba(cr, date_r, date_g, date_b, date_a);
            pango_layout_set_text(date_layout, sms->date, -1);
            pango_layout_get_pixel_size(date_layout, &w2, &h2);
            cairo_move_to(cr, margin_left + w3 + margin_address, height);
            pango_cairo_show_layout(cr, date_layout);
        } else {
            /* right */
            // draw date
            cairo_set_source_rgba(cr, date_r, date_g, date_b, date_a);
            pango_layout_set_text(date_layout, sms->date, -1);
            pango_layout_get_pixel_size(date_layout, &w2, &h2);
            cairo_move_to(cr, width - margin_right - w2 + offset, height);
            pango_cairo_show_layout(cr, date_layout);

            // draw address
            if (show_address) {
                cairo_set_source_rgba(cr, address_r, address_g, address_b,
                                      address_a);
                pango_layout_set_text(address_layout, sms->address, -1);
                pango_layout_get_pixel_size(address_layout, &w3, &h3);
                cairo_move_to(cr,
                              width - margin_right - w2 - w3 -
                              margin_address + offset, height);
                pango_cairo_show_layout(cr, address_layout);
            }
        }


        height += h2 + margin_date;

        // draw sms body
        cairo_set_source_rgba(cr, body_r, body_g, body_b, body_a);
        pango_layout_set_text(body_layout, sms->body, -1);
        pango_layout_get_pixel_size(body_layout, &w1, &h1);
        if (offset > 0 && h1 < text_width) {
            // alignment
            cairo_move_to(cr,
                          margin_left + margin_text + offset + text_width -
                          w1, height + margin_text);
            offset += text_width - w1;
        } else {
            cairo_move_to(cr, margin_left + margin_text,
                          height + margin_text);
        }
        pango_cairo_show_layout(cr, body_layout);
        h1 = h1 + margin_text * 2;

        // draw dialog box
        on_draw_box(cr, margin_left + offset, height, w1 + margin_text * 2,
                    h1, offset == 0);

        height += h1 + message_spacing;

        list = g_list_next(list);
    }
    pango_font_description_free(body_font);
    pango_font_description_free(date_font);
    pango_font_description_free(address_font);
    g_object_unref(date_layout);
    g_object_unref(body_layout);
    g_object_unref(address_layout);

    gtk_widget_set_size_request(GTK_WIDGET(widget), -1,
                                height + margin_bottom);
    return FALSE;
}

static void on_draw_box(cairo_t * cr, gint x, gint y, gint width,
                        gint height, gboolean left)
{
    if (width <= 0 || height <= 0) {
        return;
    }
    gint radius = 3;
    gint offset_y1 = 8;
    gint offset_y2 = 12;
    gint offset_x = 10;
    cairo_save(cr);

    cairo_set_source_rgba(cr, 0, 0, 0, 0.2);

    gdouble M_PI3_2 = 3.0 / 2.0 * M_PI;
    gdouble M_PI2 = 2.0 * M_PI;
    gdouble M_PI1_2 = M_PI / 2.0;

    gint xc = x + radius;
    gint yc = y + radius;
    cairo_move_to(cr, x, yc);
    cairo_arc(cr, xc, yc, radius, M_PI, M_PI3_2);

    xc = x + width - radius;
    cairo_line_to(cr, xc, y);
    cairo_arc(cr, xc, yc, radius, M_PI3_2, M_PI2);

    yc = y + height - radius;
    if (!left) {
        cairo_line_to(cr, x + width, y + height - offset_y1 - offset_y2);
        cairo_line_to(cr, x + width + offset_x, y + height - offset_y1);
        cairo_line_to(cr, x + width, y + height - offset_y1);
    }
    cairo_line_to(cr, x + width, yc);
    cairo_arc(cr, xc, yc, radius, 0, M_PI1_2);

    xc = x + radius;
    cairo_line_to(cr, xc, y + height);
    cairo_arc(cr, xc, yc, radius, M_PI1_2, M_PI);

    if (left) {
        cairo_line_to(cr, x, y + height - offset_y1);
        cairo_line_to(cr, x - offset_x, y + height - offset_y1);
        cairo_line_to(cr, x, y + height - offset_y1 - offset_y2);
    }

    yc = y + radius;
    cairo_line_to(cr, x, yc);

    cairo_stroke_preserve(cr);

    cairo_set_source_rgba(cr, 0.1, 0.1, 0.1, 0.1);
    cairo_fill(cr);

    cairo_restore(cr);
}

static void inline lc_sms_view_redraw(LcSMSView * self)
{
    gtk_widget_queue_draw(GTK_WIDGET(self->priv->draw_area));
}

void lc_sms_view_set_body_font_full(LcSMSView * self,
                                    const gchar * font_name,
                                    gint font_size,
                                    LcSMSFontStyle font_style)
{
    g_free(self->priv->body_font);
    self->priv->body_font = g_strdup(font_name);
    self->priv->body_size = font_size;
    self->priv->body_style = font_style;

    lc_sms_view_redraw(self);
}

void lc_sms_view_set_body_font_family(LcSMSView * self,
                                      const gchar * font_name)
{
    g_free(self->priv->body_font);
    self->priv->body_font = g_strdup(font_name);

    lc_sms_view_redraw(self);
}

void lc_sms_view_set_body_font_size(LcSMSView * self, gint font_size)
{
    self->priv->body_size = font_size;
    lc_sms_view_redraw(self);
}

void lc_sms_view_set_body_font_style(LcSMSView * self,
                                     LcSMSFontStyle font_style)
{
    self->priv->body_style = font_style;
    lc_sms_view_redraw(self);
}

void lc_sms_view_set_date_font_full(LcSMSView * self,
                                    const gchar * font_name,
                                    gint font_size,
                                    LcSMSFontStyle font_style)
{
    g_free(self->priv->date_font);
    self->priv->date_font = g_strdup(font_name);
    self->priv->date_size = font_style;
    self->priv->date_style = font_style;

    lc_sms_view_redraw(self);
}

void lc_sms_view_set_date_font_family(LcSMSView * self,
                                      const gchar * font_name)
{
    g_free(self->priv->date_font);
    self->priv->date_font = g_strdup(font_name);
}

void lc_sms_view_set_date_font_size(LcSMSView * self, gint font_size)
{
    self->priv->date_size = font_size;
    lc_sms_view_redraw(self);
}

void lc_sms_view_set_date_font_style(LcSMSView * self,
                                     LcSMSFontStyle font_style)
{
    self->priv->date_style = font_style;
    lc_sms_view_redraw(self);
}

void lc_sms_view_set_address_font_full(LcSMSView * self,
                                       const gchar * font_name,
                                       gint font_size,
                                       LcSMSFontStyle font_style)
{
    g_free(self->priv->address_font);
    self->priv->address_font = g_strdup(font_name);
    self->priv->address_size = font_size;
    self->priv->address_style = font_style;

    lc_sms_view_redraw(self);
}

void lc_sms_view_set_address_font_family(LcSMSView * self,
                                         const gchar * font_name)
{
    g_free(self->priv->address_font);
    self->priv->address_font = g_strdup(font_name);
}

void lc_sms_view_set_address_font_size(LcSMSView * self, gint font_size)
{
    self->priv->address_size = font_size;
    lc_sms_view_redraw(self);
}

void lc_sms_view_set_address_font_style(LcSMSView * self,
                                        LcSMSFontStyle font_style)
{
    self->priv->address_style = font_style;
    lc_sms_view_redraw(self);
}

void lc_sms_view_set_margin_full(LcSMSView * self,
                                 gint margin_top,
                                 gint margin_bottom,
                                 gint margin_left,
                                 gint margin_right,
                                 gint margin_text,
                                 gint margin_date,
                                 gint margin_address, gint message_spacing)
{
    self->priv->margin_top = margin_top;
    self->priv->margin_bottom = margin_bottom;
    self->priv->margin_left = margin_left;
    self->priv->margin_right = margin_right;
    self->priv->margin_text = margin_text;
    self->priv->margin_date = margin_date;
    self->priv->margin_address = margin_address;
    self->priv->message_spacing = message_spacing;

    lc_sms_view_redraw(self);
}

void lc_sms_view_show_address(LcSMSView * self, gboolean show)
{
    self->priv->show_address = show;
    lc_sms_view_redraw(self);
}

void lc_sms_view_set_data(LcSMSView * self, GList * list)
{
    GList *copy =
        g_list_copy_deep(list, (GCopyFunc) lc_protocol_sms_copy, NULL);
    lc_sms_view_set_data_take(self, copy);
}

void lc_sms_view_set_data_take(LcSMSView * self, GList * list)
{
    g_list_free_full(self->priv->list,
                     (GDestroyNotify) lc_protocol_sms_free);
    self->priv->list = list;
    lc_sms_view_redraw(self);
}

void lc_sms_view_reverse(LcSMSView * self)
{
    self->priv->list = g_list_reverse(self->priv->list);
    lc_sms_view_redraw(self);
}
