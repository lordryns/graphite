#include <gtk/gtk.h>
#include <gtk4-layer-shell/gtk4-layer-shell.h>

static void on_activate (GtkApplication *app) 
{
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_file(provider, g_file_new_for_path("style.css"));

    GdkDisplay *display = gdk_display_get_default(); 
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GListModel *monitors = gdk_display_get_monitors(display); 

        GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "Graphite");
    //gtk_window_set_decorated(GTK_WINDOW (window), FALSE);
    //gtk_window_set_resizable(GTK_WINDOW (window), FALSE);
    
    gtk_layer_init_for_window(GTK_WINDOW (window));
    gtk_layer_set_layer(GTK_WINDOW (window), GTK_LAYER_SHELL_LAYER_TOP);
    gtk_layer_auto_exclusive_zone_enable(GTK_WINDOW (window));

    GdkMonitor *monitor = GDK_MONITOR (g_list_model_get_item(monitors, 0));
    GdkRectangle geometry; 
    gdk_monitor_get_geometry(monitor, &geometry);

    gtk_window_set_default_size(GTK_WINDOW (window), 15, geometry.height - 100);

    // gtk_layer_set_margin(GTK_WINDOW (window), GTK_LAYER_SHELL_EDGE_LEFT, 20);
    // gtk_layer_set_margin(GTK_WINDOW (window), GTK_LAYER_SHELL_EDGE_RIGHT, 20);
    // gtk_layer_set_margin(GTK_WINDOW (window), GTK_LAYER_SHELL_EDGE_TOP, 80);
    // gtk_layer_set_margin(GTK_WINDOW (window), GTK_LAYER_SHELL_EDGE_BOTTOM, 0); // 0 is default

    gtk_layer_set_anchor(GTK_WINDOW (window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
    gtk_layer_set_margin(GTK_WINDOW (window), GTK_LAYER_SHELL_EDGE_LEFT, 10);
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); 

    GtkWidget *button = gtk_button_new_with_label("Graphite");
    
    gtk_box_append(GTK_BOX (main_box), button);
    gtk_window_set_child(GTK_WINDOW (window), main_box);
    gtk_window_present(GTK_WINDOW (window));
}

int main (int argc, char **argv)
{
    GtkApplication *app = gtk_application_new("com.lordryns.graphite", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK (on_activate), NULL);

    return g_application_run(G_APPLICATION (app), argc, argv);
    return 0;
}
