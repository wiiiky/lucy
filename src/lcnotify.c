/*
 * lcnotify.c
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

#include "lcnotify.h"
#include "lcutil.h"
#include <libnotify/notify.h>

void lc_notify_show_full(const gchar * summary, const gchar * body,
                         const gchar * icon)
{
    NotifyNotification *notification =
        notify_notification_new(summary, body,
                                icon);
    notify_notification_show(notification, NULL);

    g_object_unref(notification);
}

/*
 * show a notification message with default icon
 */
void lc_notify_show(const gchar * summary, const gchar * body)
{
    NotifyNotification *notification =
        notify_notification_new(summary, body,
                                NULL);
    GdkPixbuf *pixbuf =
        lc_util_load_pixbuf_from_resouce("default-icon.png");
    notify_notification_set_image_from_pixbuf(notification, pixbuf);

    notify_notification_show(notification, NULL);

    g_object_unref(pixbuf);
    g_object_unref(notification);
}
