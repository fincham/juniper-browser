#include "juniper-ui.h"

static GtkWindow * window;
static GtkStatusbar * status_bar;
static guint status_bar_context_id;
static GtkDialog * about_dialog;

void juniper_ui_init(GtkWindow * i_window, GtkStatusbar * i_status_bar, GtkDialog * i_about_dialog)
{
    window = i_window;
    status_bar = i_status_bar;
    status_bar_context_id = gtk_statusbar_get_context_id(status_bar, "juniper");
    about_dialog = i_about_dialog;
}

GtkWindow * juniper_ui_get_window()
{
    return window;
}

void juniper_ui_show_message_box(GtkMessageType type, GtkButtonsType buttons, const gchar *title, const gchar *message)
{
    GtkMessageDialog *message_dialog;

    message_dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(window, GTK_DIALOG_MODAL, type, buttons, title));
    gtk_message_dialog_format_secondary_text(message_dialog, "%s", message);
    gtk_dialog_run(GTK_DIALOG(message_dialog));
    gtk_widget_destroy(GTK_WIDGET(message_dialog));
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
    gtk_statusbar_push(status_bar, status_bar_context_id, text);
}

GtkDialog * juniper_ui_get_about_dialog()
{
    return about_dialog;
}
