#include <gtk/gtk.h>
#include <gtk4-layer-shell/gtk4-layer-shell.h>
#include <stdio.h>
#include "glib-object.h"
#include "log.h"
#include "ini.h"
#include <stdbool.h>


struct slot {
    char *name; 
    char *on_click_command;
};

typedef struct
{
    const char *style;
    struct slot slot1;
    struct slot slot2;
    struct slot slot3;
    struct slot slot4;
    struct slot slot5;
} configuration;

static int handler(void* user, const char* section, const char* name,
                   const char* value)
{
    configuration* pconfig = (configuration*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0 
    if (MATCH ("", "style")) {
        pconfig->style = strdup(value);
    }
    else if (MATCH("slot-1", "name")) {
        pconfig->slot1.name = strdup(value);
    }  else if (MATCH("slot-1", "on_click_command")) {
        pconfig->slot1.on_click_command = strdup(value);
    } else if (MATCH("slot-2", "name")) {
        pconfig->slot2.name = strdup(value);
    } else if (MATCH("slot-2", "on_click_command")) {
        pconfig->slot2.on_click_command = strdup(value);
    }  else if (MATCH("slot-3", "name")) {
        pconfig->slot3.name = strdup(value);
    }  else if (MATCH("slot-3", "on_click_command")) {
        pconfig->slot3.on_click_command = strdup(value);
    } else if (MATCH("slot-4", "name")) {
        pconfig->slot4.name = strdup(value);
    }  else if (MATCH("slot-4", "on_click_command")) {
        pconfig->slot4.on_click_command = strdup(value);
    } else if (MATCH("slot-5", "name")) {
        pconfig->slot5.name = strdup(value);
    }  else if (MATCH("slot-5", "on_click_command")) {
        pconfig->slot5.on_click_command = strdup(value);
    }
    else {
        return 0;
    }
    return 1;
}

static GtkWidget *popup_window = NULL;

void on_window_create_signal (GtkButton *button, gpointer user_data)
{
    GtkWindow *main_window = (GtkWindow *) user_data;
    
    if (popup_window != NULL) 
    {
        gtk_window_destroy(GTK_WINDOW (popup_window));
        popup_window = NULL;
        gtk_button_set_icon_name(button, "open-menu-symbolic");
    } 
    else 
    {
        gtk_button_set_icon_name(button, "window-close-symbolic");
        popup_window = gtk_window_new();
        gtk_widget_set_name(popup_window, "popup-window");
        gtk_window_set_transient_for(GTK_WINDOW (popup_window), main_window);
        gtk_window_set_default_size(GTK_WINDOW (popup_window), 400, 600);
        gtk_layer_init_for_window(GTK_WINDOW (popup_window));
        gtk_layer_set_layer(GTK_WINDOW (popup_window), GTK_LAYER_SHELL_LAYER_TOP);
        gtk_layer_set_anchor(GTK_WINDOW (popup_window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
    gtk_layer_set_margin(GTK_WINDOW (popup_window), GTK_LAYER_SHELL_EDGE_LEFT, 5);
        
        g_signal_connect(popup_window, "destroy", G_CALLBACK(gtk_window_destroy), &popup_window);
        
        gtk_window_present(GTK_WINDOW (popup_window));
    }
}

bool use_config = true;
static void on_activate (GtkApplication *app) 
{
    configuration config = {0}; 

    if (ini_parse("graphite.ini", handler, &config) < 0) {
        log_warn("Unable to load graphite.ini, does it exist?");
        use_config = false;
    } else {
        log_info("Loaded graphite.ini successfully");
    }

    GtkCssProvider *provider = gtk_css_provider_new();
    // config.style contains garbage if not initialised, later fix 
    gtk_css_provider_load_from_file(provider, g_file_new_for_path(config.style));

    GdkDisplay *display = gdk_display_get_default(); 
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GListModel *monitors = gdk_display_get_monitors(display); 

        GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "Graphite");
    //gtk_window_set_decorated(GTK_WINDOW (window), FALSE);
    //gtk_window_set_resizable(GTK_WINDOW (window), FALSE);
    
    gtk_widget_set_name(window, "window");
    gtk_layer_init_for_window(GTK_WINDOW (window));
    gtk_layer_set_layer(GTK_WINDOW (window), GTK_LAYER_SHELL_LAYER_TOP);
    gtk_layer_auto_exclusive_zone_enable(GTK_WINDOW (window));

    GdkMonitor *monitor = GDK_MONITOR (g_list_model_get_item(monitors, 0));
    GdkRectangle geometry; 
    gdk_monitor_get_geometry(monitor, &geometry);

    gtk_window_set_default_size(GTK_WINDOW (window), 15, geometry.height - 100);

    gtk_layer_set_anchor(GTK_WINDOW (window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
    gtk_layer_set_margin(GTK_WINDOW (window), GTK_LAYER_SHELL_EDGE_LEFT, 5);
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); 

    // apparently these null guards dont work as well as i thought they did 
    // .name returns garbage not null

    GtkWidget *expand_button = gtk_button_new_from_icon_name("open-menu-symbolic");
    gtk_widget_set_tooltip_markup(expand_button, "Expand/Collapse");
    gtk_widget_set_name(expand_button, "expand");
    g_signal_connect(expand_button, "clicked", G_CALLBACK(on_window_create_signal), GTK_WINDOW (window));
    gtk_box_append(GTK_BOX (main_box), expand_button);

    if (config.slot1.name != NULL) 
    {
        GtkWidget *s1button = gtk_button_new_from_icon_name(config.slot1.name); 
        gtk_widget_set_name(s1button, "slot");
        gtk_box_append(GTK_BOX (main_box), s1button);
    }
    if (config.slot2.name != NULL) 
    {
        GtkWidget *s2button = gtk_button_new_from_icon_name(config.slot2.name); 
        gtk_widget_set_name(s2button, "slot");
        gtk_box_append(GTK_BOX (main_box), s2button);
    }
    if (config.slot3.name != NULL)
    {
        GtkWidget *s3button = gtk_button_new_from_icon_name(config.slot3.name); 
        gtk_widget_set_name(s3button, "slot");
        gtk_box_append(GTK_BOX (main_box), s3button);
    }
    if (config.slot4.name != NULL)
    {
        GtkWidget *s4button = gtk_button_new_from_icon_name(config.slot4.name); 
        gtk_widget_set_name(s4button, "slot");
        gtk_box_append(GTK_BOX (main_box), s4button);
    }
    if (config.slot5.name != NULL)
    {
        GtkWidget *s5button = gtk_button_new_from_icon_name(config.slot5.name); 
        gtk_widget_set_name(s5button, "slot");
        gtk_box_append(GTK_BOX (main_box), s5button);
    }

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
