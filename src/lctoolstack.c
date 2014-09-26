/* lctoolstack.c generated by valac 0.22.1, the Vala compiler
 * generated from lctoolstack.vala, do not modify */


#include <glib.h>
#include <glib-object.h>
#include "lctoolstack.h"
#include <gtk/gtk.h>

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

struct _LcToolStackPrivate {
    GtkToolbar *toolbar;
    GtkStack *stack;
    guint name;
};


#define G_OBJECT_KEY_STACK_NAME "stack-name"
#define G_OBJECT_KEY_CALLBACK	"toggled-callback"
#define G_OBJECT_KEY_USER_DATA	"toggled-callback-data"

static gpointer lc_tool_stack_parent_class = NULL;

#define LC_TOOL_STACK_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_LC_TOOL_STACK, LcToolStackPrivate))
enum {
    LC_TOOL_STACK_DUMMY_PROPERTY
};
static void lc_tool_stack_finalize(GObject * obj);


LcToolStack *lc_tool_stack_construct(GType object_type)
{
    LcToolStack *self = NULL;
    self = (LcToolStack *) g_object_new(object_type, NULL);
    self->priv->toolbar = (GtkToolbar *) gtk_toolbar_new();
    gtk_toolbar_set_style(self->priv->toolbar, GTK_TOOLBAR_BOTH);
    g_object_ref_sink(self->priv->toolbar);
    self->priv->stack = (GtkStack *) gtk_stack_new();
    gtk_stack_set_transition_type(self->priv->stack,
                                  GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    g_object_ref_sink(self->priv->stack);

    gtk_grid_attach(GTK_GRID(self), GTK_WIDGET(self->priv->toolbar), 0, 0,
                    1, 1);
    gtk_grid_attach(GTK_GRID(self), GTK_WIDGET(self->priv->stack), 0, 1, 1,
                    1);
    gtk_widget_set_vexpand(GTK_WIDGET(self->priv->stack), TRUE);

    self->priv->name = 0;
    return self;
}


LcToolStack *lc_tool_stack_new(void)
{
    return lc_tool_stack_construct(TYPE_LC_TOOL_STACK);
}


static void lc_tool_stack_class_init(LcToolStackClass * klass)
{
    lc_tool_stack_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(LcToolStackPrivate));
    G_OBJECT_CLASS(klass)->finalize = lc_tool_stack_finalize;
}


static void lc_tool_stack_instance_init(LcToolStack * self)
{
    self->priv = LC_TOOL_STACK_GET_PRIVATE(self);

    gtk_grid_set_column_homogeneous(GTK_GRID(self), TRUE);
}


static void lc_tool_stack_finalize(GObject * obj)
{
    LcToolStack *self;
    self =
        G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_LC_TOOL_STACK, LcToolStack);
    _g_object_unref0(self->priv->toolbar);
    _g_object_unref0(self->priv->stack);
    G_OBJECT_CLASS(lc_tool_stack_parent_class)->finalize(obj);
}


GType lc_tool_stack_get_type(void)
{
    static volatile gsize lc_tool_stack_type_id__volatile = 0;
    if (g_once_init_enter(&lc_tool_stack_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(LcToolStackClass), (GBaseInitFunc) NULL,
(GBaseFinalizeFunc) NULL, (GClassInitFunc) lc_tool_stack_class_init, (GClassFinalizeFunc) NULL,
NULL, sizeof(LcToolStack), 0, (GInstanceInitFunc) lc_tool_stack_instance_init, NULL };
        GType lc_tool_stack_type_id;
        lc_tool_stack_type_id =
            g_type_register_static(GTK_TYPE_GRID, "LcToolStack",
                                   &g_define_type_info, 0);
        g_once_init_leave(&lc_tool_stack_type_id__volatile,
                          lc_tool_stack_type_id);
    }
    return lc_tool_stack_type_id__volatile;
}

static void onRadioToolButtonToggled(GtkRadioToolButton * radio,
                                     gpointer user_data)
{
    LcToolStack *self = LC_TOOL_STACK(user_data);
    gboolean toggled =
        gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(radio));

    const gchar *stack_name =
        (gchar *) g_object_get_data(G_OBJECT(radio),
                                    G_OBJECT_KEY_STACK_NAME);
    LcToolStackToggledNotify callback =
        (LcToolStackToggledNotify) g_object_get_data(G_OBJECT(radio),
                                                     G_OBJECT_KEY_CALLBACK);
    gpointer data =
        g_object_get_data(G_OBJECT(radio), G_OBJECT_KEY_USER_DATA);
    if (toggled) {
        gtk_stack_set_visible_child_name(self->priv->stack, stack_name);
    }
    if (callback) {
        callback(toggled, data);
    }
}

void lc_tool_stack_append(LcToolStack * self,
                          GtkWidget * title_widget,
                          const gchar * title_label,
                          GtkWidget * content,
                          LcToolStackToggledNotify callback,
                          gpointer user_data)
{
    GtkToolbar *toolbar = self->priv->toolbar;
    GtkStack *stack = self->priv->stack;
    GtkRadioToolButton *radio =
        (GtkRadioToolButton *) gtk_toolbar_get_nth_item(toolbar, 0);

    GtkToolItem *button = gtk_radio_tool_button_new_from_widget(radio);
    gtk_tool_button_set_label_widget(GTK_TOOL_BUTTON(button),
                                     title_widget);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(button), title_label);
    gtk_toolbar_insert(toolbar, button, -1);

    gchar name[8];
    g_snprintf(name, sizeof(name), "%u", self->priv->name++);
    gtk_stack_add_named(stack, content, name);

    g_object_set_data_full(G_OBJECT(button), G_OBJECT_KEY_STACK_NAME,
                           g_strdup(name), (GDestroyNotify) g_free);
    g_object_set_data(G_OBJECT(button), G_OBJECT_KEY_CALLBACK, callback);
    g_object_set_data(G_OBJECT(button), G_OBJECT_KEY_USER_DATA, user_data);

    g_signal_connect(G_OBJECT(button), "toggled",
                     G_CALLBACK(onRadioToolButtonToggled), self);
}


void lc_tool_stack_set_transition_duration(LcToolStack * self,
                                           guint duration)
{
    gtk_stack_set_transition_duration(self->priv->stack, duration);
}

void lc_tool_stack_set_transition_type(LcToolStack * self,
                                       GtkStackTransitionType type)
{
    gtk_stack_set_transition_type(self->priv->stack, type);
}