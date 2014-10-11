/*
 * lcsmsbox.c
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
#include "lcsmsbox.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct _LcSmsBoxPrivate {
    gchar *text;
};

#define lc_sms_box_get_priv(self) ((self)->priv)

static gpointer lc_sms_box_parent_class = NULL;

#define LC_SMS_BOX_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_LC_SMS_BOX, LcSmsBoxPrivate))
enum {
    LC_SMS_BOX_DUMMY_PROPERTY
};
static void lc_sms_box_finalize(GObject * obj);
static void on_draw(GtkDrawingArea * area, cairo_t * cr,
                    gpointer user_data);


LcSmsBox *lc_sms_box_construct(GType object_type)
{
    LcSmsBox *self = NULL;
    self = (LcSmsBox *) g_object_new(object_type, NULL);
    return self;
}


LcSmsBox *lc_sms_box_new(void)
{
    return lc_sms_box_construct(TYPE_LC_SMS_BOX);
}

LcSmsBox *lc_sms_box_new_with_text(const gchar * text)
{
    LcSmsBox *self = lc_sms_box_construct(TYPE_LC_SMS_BOX);
    lc_sms_box_set_text(self, text);
    return self;
}

static void lc_sms_box_class_init(LcSmsBoxClass * klass)
{
    lc_sms_box_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(LcSmsBoxPrivate));
    G_OBJECT_CLASS(klass)->finalize = lc_sms_box_finalize;
}


static void lc_sms_box_instance_init(LcSmsBox * self)
{
    self->priv = LC_SMS_BOX_GET_PRIVATE(self);

    g_signal_connect(G_OBJECT(self), "draw", G_CALLBACK(on_draw), self);

    LcSmsBoxPrivate *priv = self->priv;

    priv->text = g_strdup("");
}


static void lc_sms_box_finalize(GObject * obj)
{
    LcSmsBox *self;
    self = G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_LC_SMS_BOX, LcSmsBox);
    g_free(self->priv->text);
    G_OBJECT_CLASS(lc_sms_box_parent_class)->finalize(obj);
}

static void on_draw(GtkDrawingArea * area, cairo_t * cr,
                    gpointer user_data)
{
    LcSmsBox *self = (LcSmsBox *) user_data;

    gint width = gtk_widget_get_allocated_width(GTK_WIDGET(self));
    gint height = gtk_widget_get_allocated_height(GTK_WIDGET(self));

    gint BORDER_SPACING = 25;   //the spacing between border and text
    static PangoFontDescription *font = NULL;
    if (font == NULL) {
        font = pango_font_description_new();
        pango_font_description_set_style(font, PANGO_STYLE_ITALIC);
        pango_font_description_set_family(font, "ubuntu1 mono");
    }

    PangoLayout *layout = pango_cairo_create_layout(cr);

    pango_layout_set_font_description(layout, font);
    pango_layout_set_text(layout, lc_sms_box_get_text(self), -1);
    pango_layout_set_width(layout,
                           (width - BORDER_SPACING * 2) * PANGO_SCALE);
    pango_layout_set_wrap(layout, PANGO_WRAP_CHAR);
    cairo_move_to(cr, BORDER_SPACING, BORDER_SPACING);
    pango_cairo_show_layout(cr, layout);

    PangoRectangle rect;
    pango_layout_get_pixel_extents(layout, NULL, &rect);

    gint TEXT_SPACING = 10;     // the spacing between text and lines
    gint x1 = BORDER_SPACING - TEXT_SPACING, y1 =
        BORDER_SPACING - TEXT_SPACING;
    gint x2 = BORDER_SPACING + rect.width + TEXT_SPACING, y2 =
        BORDER_SPACING - TEXT_SPACING;
    gint x3 = BORDER_SPACING + rect.width + TEXT_SPACING, y3 =
        BORDER_SPACING + rect.height + TEXT_SPACING;
    gint x4 = BORDER_SPACING - TEXT_SPACING, y4 =
        BORDER_SPACING + rect.height + TEXT_SPACING;

    cairo_set_source_rgba(cr, 0.3, 0.3, 0.25, 1);

    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    cairo_line_to(cr, x3, y3);
    cairo_line_to(cr, x4, y4);
    cairo_close_path(cr);

    cairo_stroke(cr);
}

GType lc_sms_box_get_type(void)
{
    static volatile gsize lc_sms_box_type_id__volatile = 0;
    if (g_once_init_enter(&lc_sms_box_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(LcSmsBoxClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
                (GClassInitFunc) lc_sms_box_class_init,
                (GClassFinalizeFunc) NULL, NULL,
            sizeof(LcSmsBox), 0,
                (GInstanceInitFunc) lc_sms_box_instance_init, NULL
        };
        GType lc_sms_box_type_id;
        lc_sms_box_type_id =
            g_type_register_static(GTK_TYPE_DRAWING_AREA, "LcSmsBox",
                                   &g_define_type_info, 0);
        g_once_init_leave(&lc_sms_box_type_id__volatile,
                          lc_sms_box_type_id);
    }
    return lc_sms_box_type_id__volatile;
}


const gchar *lc_sms_box_get_text(LcSmsBox * self)
{
    LcSmsBoxPrivate *priv = lc_sms_box_get_priv(self);
    return priv->text;
}

void lc_sms_box_set_text(LcSmsBox * self, const gchar * text)
{
    LcSmsBoxPrivate *priv = lc_sms_box_get_priv(self);
    g_free(priv->text);
    priv->text = g_strdup(text);
}
