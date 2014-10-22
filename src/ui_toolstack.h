/*
 * ui_toolstack.h
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



#ifndef __UI_TOOLSTACK_H__
#define __UI_TOOLSTACK_H__

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS
#define TYPE_UI_TOOL_STACK (ui_tool_stack_get_type ())
#define UI_TOOL_STACK(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_UI_TOOL_STACK, UIToolStack))
#define UI_TOOL_STACK_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_UI_TOOL_STACK, UIToolStackClass))
#define IS_UI_TOOL_STACK(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_UI_TOOL_STACK))
#define IS_UI_TOOL_STACK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_UI_TOOL_STACK))
#define UI_TOOL_STACK_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_UI_TOOL_STACK, UIToolStackClass))
typedef struct _UIToolStack UIToolStack;
typedef struct _UIToolStackClass UIToolStackClass;
typedef struct _UIToolStackPrivate UIToolStackPrivate;

struct _UIToolStack {
    GtkGrid parent_instance;
    UIToolStackPrivate *priv;
};

struct _UIToolStackClass {
    GtkGridClass parent_class;
};


GType ui_tool_stack_get_type(void) G_GNUC_CONST;
UIToolStack *ui_tool_stack_new(void);
UIToolStack *ui_tool_stack_construct(GType object_type);

typedef void (*UIToolStackToggledNotify) (gboolean toggled,
                                          gpointer user_data);

void ui_tool_stack_append(UIToolStack * self,
                          GtkWidget * title_widget,
                          const gchar * title_label,
                          GtkWidget * content,
                          UIToolStackToggledNotify callback,
                          gpointer user_data);

/*
 * return the title of current page
 */
const gchar *ui_tool_stack_get_current_title(UIToolStack * self);

/*
 * wrapper for gtk_stack_set_*
 */
void ui_tool_stack_set_transition_duration(UIToolStack * self,
                                           guint duration);
void ui_tool_stack_set_transition_type(UIToolStack * self,
                                       GtkStackTransitionType type);

G_END_DECLS
#endif
