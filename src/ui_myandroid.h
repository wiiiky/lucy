/*
 * ui_myandroid.h
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

#ifndef __UI_MYANDROID_H__
#define __UI_MYANDROID_H__

#include <glib.h>
#include <gtk/gtk.h>

#include "lcprotocol.h"

G_BEGIN_DECLS
#define TYPE_UI_MY_ANDROID (ui_my_android_get_type ())
#define UI_MY_ANDROID(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_UI_MY_ANDROID, UIMyAndroid))
#define UI_MY_ANDROID_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_UI_MY_ANDROID, UIMyAndroidClass))
#define IS_UI_MY_ANDROID(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_UI_MY_ANDROID))
#define IS_UI_MY_ANDROID_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_UI_MY_ANDROID))
#define UI_MY_ANDROID_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_UI_MY_ANDROID, UIMyAndroidClass))
typedef struct _UIMyAndroid UIMyAndroid;
typedef struct _UIMyAndroidClass UIMyAndroidClass;
typedef struct _UIMyAndroidPrivate UIMyAndroidPrivate;

struct _UIMyAndroid {
    GtkStack parent_instance;
    UIMyAndroidPrivate *priv;
};

struct _UIMyAndroidClass {
    GtkStackClass parent_class;
};


GType ui_my_android_get_type(void) G_GNUC_CONST;
UIMyAndroid *ui_my_android_new(void);
UIMyAndroid *ui_my_android_construct(GType object_type);

void ui_my_android_show_disconnect(UIMyAndroid * self);
void ui_my_android_show_connecting(UIMyAndroid * self);
void ui_my_android_show_connected(UIMyAndroid * self);
void ui_my_android_show_connected_with_info(UIMyAndroid * self,
                                            LcProtocolPhone * phone);

void ui_my_android_set_connect_callback(UIMyAndroid * self,
                                        GCallback callback,
                                        gpointer user_data);

G_END_DECLS
#endif
