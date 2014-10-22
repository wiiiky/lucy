/*
 * ui_mainwindow.h
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



#ifndef __UI_MAINWINDOW_H__
#define __UI_MAINWINDOW_H__

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS
#define TYPE_UI_MAIN_WINDOW (ui_main_window_get_type ())
#define UI_MAIN_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_UI_MAIN_WINDOW, UIMainWindow))
#define UI_MAIN_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_UI_MAIN_WINDOW, UIMainWindowClass))
#define IS_UI_MAIN_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_UI_MAIN_WINDOW))
#define IS_UI_MAIN_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_UI_MAIN_WINDOW))
#define UI_MAIN_WINDOW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_UI_MAIN_WINDOW, UIMainWindowClass))
typedef struct _UIMainWindow UIMainWindow;
typedef struct _UIMainWindowClass UIMainWindowClass;
typedef struct _UIMainWindowPrivate UIMainWindowPrivate;

struct _UIMainWindow {
    GtkWindow parent_instance;
    UIMainWindowPrivate *priv;
};

struct _UIMainWindowClass {
    GtkWindowClass parent_class;
};


GType ui_main_window_get_type(void) G_GNUC_CONST;
UIMainWindow *ui_main_window_new(void);
UIMainWindow *ui_main_window_construct(GType object_type);


void ui_main_window_show(UIMainWindow * window);

/*
 * 启动adb服务器
 */
void ui_main_window_start_server(UIMainWindow * window);


G_END_DECLS
#endif
