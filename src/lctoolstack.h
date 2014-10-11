/*
 * lctoolstack.h
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



#ifndef __LCTOOLSTACK_H__
#define __LCTOOLSTACK_H__

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS
#define TYPE_LC_TOOL_STACK (lc_tool_stack_get_type ())
#define LC_TOOL_STACK(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_LC_TOOL_STACK, LcToolStack))
#define LC_TOOL_STACK_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_LC_TOOL_STACK, LcToolStackClass))
#define IS_LC_TOOL_STACK(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_LC_TOOL_STACK))
#define IS_LC_TOOL_STACK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_LC_TOOL_STACK))
#define LC_TOOL_STACK_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_LC_TOOL_STACK, LcToolStackClass))
typedef struct _LcToolStack LcToolStack;
typedef struct _LcToolStackClass LcToolStackClass;
typedef struct _LcToolStackPrivate LcToolStackPrivate;

struct _LcToolStack {
    GtkGrid parent_instance;
    LcToolStackPrivate *priv;
};

struct _LcToolStackClass {
    GtkGridClass parent_class;
};


GType lc_tool_stack_get_type(void) G_GNUC_CONST;
LcToolStack *lc_tool_stack_new(void);
LcToolStack *lc_tool_stack_construct(GType object_type);

typedef void (*LcToolStackToggledNotify) (gboolean toggled,
                                          gpointer user_data);

void lc_tool_stack_append(LcToolStack * self,
                          GtkWidget * title_widget,
                          const gchar * title_label,
                          GtkWidget * content,
                          LcToolStackToggledNotify callback,
                          gpointer user_data);

/*
 * wrapper for gtk_stack_set_*
 */
void lc_tool_stack_set_transition_duration(LcToolStack * self,
                                           guint duration);
void lc_tool_stack_set_transition_type(LcToolStack * self,
                                       GtkStackTransitionType type);

G_END_DECLS
#endif
