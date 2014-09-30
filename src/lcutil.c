/*
 * lcutil.c
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
#include "lcutil.h"
#include <limits.h>
#include <unistd.h>

static char exedir[PATH_MAX + 1] = "./";

void lc_init(int argc, char *argv[])
{
    /* get the running executable file's absolute path */
    ssize_t count = readlink("/proc/self/exe", exedir, PATH_MAX);
    if (count <= 0) {
        return;
    }
    exedir[count] = '\0';
    gchar *s = g_strrstr(exedir, "/");
    if (s) {
        *s = '\0';
    }
}

gchar *lc_util_get_string_from_byte_array(GByteArray * array, gsize * size)
{
    if (array == NULL) {
        return NULL;
    }
    guint8 null = '\0';
    g_byte_array_append(array, &null, 1);
    GBytes *bytes = g_byte_array_free_to_bytes(array);
    gsize len;
    if (size == NULL) {
        size = &len;
    }
    return g_bytes_unref_to_data(bytes, size);
}

GBytes *lc_util_get_bytes_from_byte_array(GByteArray * array)
{
    if (array == NULL) {
        return NULL;
    }
    GBytes *bytes = g_byte_array_free_to_bytes(array);

    return bytes;
}

const gchar *lc_util_get_resource_by_name(const gchar * name)
{
    static gchar buf[1024];
    g_snprintf(buf, sizeof(buf), "%s/%s", PACKAGE_DATA_DIR, name);
    if (g_file_test(buf, G_FILE_TEST_EXISTS)) {
        return buf;
    }
    g_snprintf(buf, sizeof(buf), "%s/res/%s", exedir, name);
    if (g_file_test(buf, G_FILE_TEST_EXISTS)) {
        return buf;
    }
    g_warning("Resource '%s' not found", name);
    return NULL;
}

static void apply_css_provider_forall(GtkWidget * widget, gpointer data)
{
    GtkCssProvider *provider = (GtkCssProvider *) data;
    if (GTK_IS_CONTAINER(widget)) {
        gtk_container_forall(GTK_CONTAINER(widget),
                             (GtkCallback) apply_css_provider_forall,
                             data);
    }
    GtkStyleContext *style =
        gtk_widget_get_style_context(GTK_WIDGET(widget));
    gtk_style_context_add_provider(style, GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void lc_util_load_css(GtkWidget * widget, const gchar * name)
{
    const gchar *path = lc_util_get_resource_by_name(name);
    GtkCssProvider *provider = gtk_css_provider_new();
    if (!gtk_css_provider_load_from_path(provider, path, NULL)) {
        g_warning("Failed to load CSS file: %s", path);
    } else {
        apply_css_provider_forall(widget, provider);
    }
    g_object_unref(provider);
}

GList *lc_util_clean_list_by(GList * list, GListElementNotify judge,
                             gpointer user_data, GDestroyNotify destroy)
{
    GList *lp = list;
    while (lp) {
        GList *next = g_list_next(lp);
        if (judge(lp->data, user_data)) {
            if (lp == list) {   /* first element */
                list = next;
                list->prev = NULL;
                destroy(lp->data);
                g_list_free_1(lp);
            } else {
                lp->prev->next = next;
                next->prev = lp->prev;
                destroy(lp->data);
                g_list_free_1(lp);
            }
        }
        lp = next;
    }
    return list;
}
