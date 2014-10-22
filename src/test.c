#include <gtk/gtk.h>
#include "ui_smsview.h"
#include "ui_smsrow.h"
#include "lcprotocol.h"

static GtkWidget *window = NULL;

static void close_window()
{
    if (window) {
        gtk_widget_destroy(window);
        window = NULL;
    }
}

int show_test(GList * list)
{
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "TEST ONLY");
    gtk_window_set_default_size(GTK_WINDOW(window), 960, 640);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(close_window), NULL);

    UISMSView *view = ui_sms_view_new_reverse(list);
    UISMSRow *row = ui_sms_row_new(list);
    ui_sms_row_highlight(row);

    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_paned_pack1(GTK_PANED(paned), GTK_WIDGET(row), TRUE, TRUE);
    gtk_paned_pack2(GTK_PANED(paned), GTK_WIDGET(view), FALSE, FALSE);
    gtk_container_add(GTK_CONTAINER(window), paned);

    gtk_widget_show_all(window);
    return 0;
}
