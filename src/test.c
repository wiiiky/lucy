#include <gtk/gtk.h>
#include "lcsmsview.h"
#include "lcprotocol.h"

int show_test(GList * list)
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "TEST ONLY");
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    LcSMSView *view = lc_sms_view_new(list);
    lc_sms_view_reverse(view);
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(view));

    gtk_widget_show_all(window);
    return 0;
}
