/*
 * lcmainwindow.h
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



#ifndef __LCMAINWINDOW_H__
#define __LCMAINWINDOW_H__

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS
#define TYPE_LC_MAIN_WINDOW (lc_main_window_get_type ())
#define LC_MAIN_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_LC_MAIN_WINDOW, LcMainWindow))
#define LC_MAIN_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_LC_MAIN_WINDOW, LcMainWindowClass))
#define IS_LC_MAIN_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_LC_MAIN_WINDOW))
#define IS_LC_MAIN_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_LC_MAIN_WINDOW))
#define LC_MAIN_WINDOW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_LC_MAIN_WINDOW, LcMainWindowClass))
typedef struct _LcMainWindow LcMainWindow;
typedef struct _LcMainWindowClass LcMainWindowClass;
typedef struct _LcMainWindowPrivate LcMainWindowPrivate;

struct _LcMainWindow {
    GtkWindow parent_instance;
    LcMainWindowPrivate *priv;
};

struct _LcMainWindowClass {
    GtkWindowClass parent_class;
};


GType lc_main_window_get_type(void) G_GNUC_CONST;
LcMainWindow *lc_main_window_new(void);
LcMainWindow *lc_main_window_construct(GType object_type);


void lc_main_window_show(LcMainWindow * window);

/*
 * 启动adb服务器
 */
void lc_main_window_start_server(LcMainWindow * window);


G_END_DECLS
#endif
