/*
 * ui_smsview.c
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


#include "ui_smsview.h"
#include "ui_smsbox.h"
#include "ui_smsrow.h"
#include "lcprotocol.h"

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

struct _UISMSViewPrivate {
    GtkPaned *sms_paned;
    UISMSBox *sms_box;
    GtkGrid *sms_grid;
    gint sms_count;
};
#define STACK_NAME_SMS	"sms"


static gpointer ui_sms_view_parent_class = NULL;

#define UI_SMS_VIEW_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), UI_SMS_TYPE_VIEW, UISMSViewPrivate))
enum {
    UI_SMS_VIEW_DUMMY_PROPERTY
};
static void ui_sms_view_finalize(GObject * obj);
static void ui_sms_view_sms_paned_init(UISMSView * self);


UISMSView *ui_sms_view_construct(GType object_type, GList * list)
{
    UISMSView *self = NULL;
    self = (UISMSView *) g_object_new(object_type, NULL);
    ui_sms_view_update(self, list);
    return self;
}


UISMSView *ui_sms_view_new(GList * list)
{
    return ui_sms_view_construct(UI_SMS_TYPE_VIEW, list);
}


static void ui_sms_view_class_init(UISMSViewClass * klass)
{
    ui_sms_view_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(UISMSViewPrivate));
    G_OBJECT_CLASS(klass)->finalize = ui_sms_view_finalize;
}


static void ui_sms_view_instance_init(UISMSView * self)
{
    self->priv = UI_SMS_VIEW_GET_PRIVATE(self);

    ui_sms_view_sms_paned_init(self);

    self->priv->sms_count = 0;
}


/*
 * 初始化短信面板
 */
static void ui_sms_view_sms_paned_init(UISMSView * self)
{
    UISMSViewPrivate *priv = self->priv;
    priv->sms_paned =
        (GtkPaned *) gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    g_object_ref_sink(priv->sms_paned);
    gtk_stack_add_named(GTK_STACK(self), GTK_WIDGET(self->priv->sms_paned),
                        STACK_NAME_SMS);

    priv->sms_grid = (GtkGrid *) gtk_grid_new();
    g_object_ref_sink(priv->sms_grid);
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled, 250, -1);
    gtk_container_add(GTK_CONTAINER(scrolled), GTK_WIDGET(priv->sms_grid));
    gtk_paned_pack1(priv->sms_paned, scrolled, TRUE, FALSE);

    priv->sms_box = ui_sms_box_new(NULL);
    g_object_ref_sink(priv->sms_box);
    gtk_paned_pack2(priv->sms_paned, GTK_WIDGET(priv->sms_box), TRUE,
                    FALSE);

}

static void ui_sms_view_finalize(GObject * obj)
{
    UISMSView *self;
    self = G_TYPE_CHECK_INSTANCE_CAST(obj, UI_SMS_TYPE_VIEW, UISMSView);
    _g_object_unref0(self->priv->sms_paned);
    _g_object_unref0(self->priv->sms_grid);
    _g_object_unref0(self->priv->sms_box);
    G_OBJECT_CLASS(ui_sms_view_parent_class)->finalize(obj);
}


GType ui_sms_view_get_type(void)
{
    static volatile gsize ui_sms_view_type_id__volatile = 0;
    if (g_once_init_enter(&ui_sms_view_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(UISMSViewClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
                (GClassInitFunc) ui_sms_view_class_init,
                (GClassFinalizeFunc) NULL, NULL,
            sizeof(UISMSView), 0,
                (GInstanceInitFunc) ui_sms_view_instance_init, NULL
        };
        GType ui_sms_view_type_id;
        ui_sms_view_type_id =
            g_type_register_static(GTK_TYPE_STACK, "UISMSView",
                                   &g_define_type_info, 0);
        g_once_init_leave(&ui_sms_view_type_id__volatile,
                          ui_sms_view_type_id);
    }
    return ui_sms_view_type_id__volatile;
}

void ui_sms_view_update(UISMSView * self, GList * list)
{
    GtkGrid *grid = self->priv->sms_grid;
    UISMSBox *box = self->priv->sms_box;
    GList *lp = list;
    gint i = 0;
    while (lp) {
        GList *l = (GList *) lp->data;
        UISMSRow *row = (UISMSRow *) gtk_grid_get_child_at(grid, 0, i);
        if (row) {
            ui_sms_row_set_data_take(row, l);
        } else {
            ui_sms_view_append_row_take(self, l);
        }
        lp = g_list_next(lp);
        i++;
    }
    /* 删除多余的 */
    while (gtk_grid_get_child_at(grid, 0, i)) {
        gtk_grid_remove_row(grid, i);
    }

    /* 显示第一个 */
    UISMSRow *row = (UISMSRow *) gtk_grid_get_child_at(grid, 0, 0);
    if (row) {
        GList *list = ui_sms_row_get_data(row);
        ui_sms_box_set_data(box, list);
    }
    g_list_free(list);
}

void ui_sms_view_append_row(UISMSView * self, GList * list)
{
    GList *copy =
        g_list_copy_deep(list, (GCopyFunc) lc_protocol_sms_copy, NULL);
    return ui_sms_view_append_row_take(self, copy);
}

void ui_sms_view_append_row_take(UISMSView * self, GList * list)
{
    GtkGrid *grid = self->priv->sms_grid;
    UISMSRow *row = ui_sms_row_new_take(list);
    gtk_grid_attach(grid, GTK_WIDGET(row), 0, self->priv->sms_count, 1, 1);
    self->priv->sms_count++;
}
