/*
 * lcmyandroid.h
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

#ifndef __LCMYANDROID_H__
#define __LCMYANDROID_H__

#include <glib.h>
#include <gtk/gtk.h>

#include "lcprotocol.h"

G_BEGIN_DECLS
#define TYPE_LC_MY_ANDROID (lc_my_android_get_type ())
#define LC_MY_ANDROID(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_LC_MY_ANDROID, LcMyAndroid))
#define LC_MY_ANDROID_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_LC_MY_ANDROID, LcMyAndroidClass))
#define IS_LC_MY_ANDROID(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_LC_MY_ANDROID))
#define IS_LC_MY_ANDROID_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_LC_MY_ANDROID))
#define LC_MY_ANDROID_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_LC_MY_ANDROID, LcMyAndroidClass))
typedef struct _LcMyAndroid LcMyAndroid;
typedef struct _LcMyAndroidClass LcMyAndroidClass;
typedef struct _LcMyAndroidPrivate LcMyAndroidPrivate;

struct _LcMyAndroid {
    GtkStack parent_instance;
    LcMyAndroidPrivate *priv;
};

struct _LcMyAndroidClass {
    GtkStackClass parent_class;
};


GType lc_my_android_get_type(void) G_GNUC_CONST;
LcMyAndroid *lc_my_android_new(void);
LcMyAndroid *lc_my_android_construct(GType object_type);

void lc_my_android_show_disconnect(LcMyAndroid * self);
void lc_my_android_show_connecting(LcMyAndroid * self);
void lc_my_android_show_connected(LcMyAndroid * self);
void lc_my_android_show_connected_with_info(LcMyAndroid * self,
                                            LcProtocolPhone * phone);

void lc_my_android_set_connect_callback(LcMyAndroid * self,
                                        GCallback callback,
                                        gpointer user_data);

G_END_DECLS
#endif
