#include "juniper-ui.h"

#include "string.h"

#include "juniper-util.h"

static GtkWindow * window;
static GtkStatusbar * status_bar;
static guint status_bar_context_id;
static GtkDialog * about_dialog;

gboolean juniper_ui_init(GladeXML * xml)
{
    window = GTK_WINDOW(glade_xml_get_widget(xml, "main_window"));

    status_bar = GTK_STATUSBAR(glade_xml_get_widget(xml, "status_bar"));
    status_bar_context_id = gtk_statusbar_get_context_id(status_bar, "juniper");

    about_dialog = GTK_DIALOG(glade_xml_get_widget(xml, "about_dialog"));

    gtk_widget_show_all(GTK_WIDGET(window));

    return TRUE;
}

GtkWindow * juniper_ui_get_window()
{
    return window;
}

void juniper_ui_set_window_title(const gchar * title)
{
    gchar * new_title;

    if (NULL == title || strlen(title) < 1)
        new_title = "Juniper";
    else
        new_title = juniper_util_sprintf("%s - Juniper", title);

    gtk_window_set_title(window, new_title);
}

gboolean juniper_ui_show_message_box(GtkMessageType type, GtkButtonsType buttons, const gchar *title, const gchar *message)
{
    GtkMessageDialog *message_dialog;
    gboolean result;

    message_dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(window, GTK_DIALOG_MODAL, type, buttons, title));
    gtk_message_dialog_format_secondary_text(message_dialog, "%s", message);
    result = gtk_dialog_run(GTK_DIALOG(message_dialog));
    gtk_widget_destroy(GTK_WIDGET(message_dialog));

    return result;
}

GtkStatusbar * juniper_ui_status_bar()
{
    return status_bar;
}

void juniper_ui_status_bar_clear()
{
    gtk_statusbar_pop(status_bar, status_bar_context_id);
}

void juniper_ui_status_bar_update(const gchar *text)
{
    juniper_ui_status_bar_clear();

    if (text != NULL)
        gtk_statusbar_push(status_bar, status_bar_context_id, text);
}

GtkDialog * juniper_ui_get_about_dialog()
{
    return about_dialog;
}
