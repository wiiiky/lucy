/* lcsmsview.c generated by valac 0.22.1, the Vala compiler
 * generated from lcsmsview.vala, do not modify */


#include "lcsmsview.h"
#include "lcprotocol.h"
#include <gtk/gtk.h>
#include <math.h>

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

struct _LcSMSViewPrivate {
    GtkDrawingArea *draw_area;
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
                        gint height);


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
}


static void lc_sms_view_finalize(GObject * obj)
{
    LcSMSView *self;
    self = G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_LC_SMS_VIEW, LcSMSView);
    _g_object_unref0(self->priv->draw_area);
    g_list_free_full(self->priv->list,
                     (GDestroyNotify) lc_protocol_sms_free);
    G_OBJECT_CLASS(lc_sms_view_parent_class)->finalize(obj);
}


GType lc_sms_view_get_type(void)
{
    static volatile gsize lc_sms_view_type_id__volatile = 0;
    if (g_once_init_enter(&lc_sms_view_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(LcSMSViewClass), (GBaseInitFunc) NULL,
(GBaseFinalizeFunc) NULL, (GClassInitFunc) lc_sms_view_class_init, (GClassFinalizeFunc) NULL, NULL,
sizeof(LcSMSView), 0, (GInstanceInitFunc) lc_sms_view_instance_init, NULL };
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
    gint width = gtk_widget_get_allocated_width(widget);
    gint margin_left = 20;
    gint margin_right = 20;
    gint margin_top = 20;
    gint margin_bottom = 20;
    gint text_margin = 10;

    gint margin_begin = 30;
    gint margin_end = 30;

    PangoFontDescription *font = pango_font_description_new();
    pango_font_description_set_size(font, 20 * PANGO_SCALE);
    pango_font_description_set_family(font, "ubuntu mono");
    pango_font_description_set_style(font, PANGO_STYLE_ITALIC);

    PangoLayout *layout = pango_cairo_create_layout(cr);
    pango_layout_set_font_description(layout, font);
    pango_layout_set_width(layout,
                           (width - margin_left - margin_right -
                            2 * text_margin) * PANGO_SCALE);
    gint height = margin_begin;
    while (list) {
        LcProtocolSMS *sms = (LcProtocolSMS *) list->data;

        cairo_set_source_rgba(cr, 0, 0, 0, 1);
        cairo_move_to(cr, margin_left + text_margin, height + text_margin);
        pango_layout_set_text(layout, sms->body, -1);

        gint h;
        pango_layout_get_size(layout, NULL, &h);
        h = h / PANGO_SCALE + text_margin * 2;

        pango_cairo_show_layout(cr, layout);
        on_draw_box(cr, margin_left, height,
                    width - margin_left - margin_right, h);

        height += h + margin_top;
        list = g_list_next(list);
    }
    pango_font_description_free(font);
    g_object_unref(layout);

    gtk_widget_set_size_request(GTK_WIDGET(widget), -1,
                                height + margin_end);
    return FALSE;
}

static void on_draw_box(cairo_t * cr, gint x, gint y, gint width,
                        gint height)
{
    if (width <= 0 || height <= 0) {
        return;
    }
    gint radius = 3;
    gint offset_y1 = 10;
    gint offset_y2 = 10;
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
    cairo_line_to(cr, x + width, yc);
    cairo_arc(cr, xc, yc, radius, 0, M_PI1_2);

    xc = x + radius;
    cairo_line_to(cr, xc, y + height);
    cairo_arc(cr, xc, yc, radius, M_PI1_2, M_PI);

    cairo_line_to(cr, x, y + height - offset_y1);
    cairo_line_to(cr, x - offset_x, y + height - offset_y1);
    cairo_line_to(cr, x, y + height - offset_y1 - offset_y2);

    yc = y + radius;
    cairo_line_to(cr, x, yc);

    cairo_stroke_preserve(cr);

    cairo_set_source_rgba(cr, 0.1, 0.1, 0.1, 0.1);
    cairo_fill(cr);

    cairo_restore(cr);
}
