#include "juniper-download.h"

#include "gtk/gtk.h"

void juniper_download(const gchar * url)
{
    GtkDialog * destination_chooser_dialog;

    GtkWindow * download_window;
    VteTerminal * terminal;
    char ** argv, ** envv;

    destination_chooser_dialog = gtk_file_chooser_dialog_new("Save File", juniper_ui_get_window(), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(destination_choose_dialog)) != GTK_RESPONSE_ACCEPT)
        return;

    download_window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    gtk_window_set_title(download_window, "Download");
    gtk_window_set_destroy_with_parent(download_window, TRUE);

    terminal = vte_terminal_new();

    argv = { "wget", "-o", gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(destination_chooser_dialog)), url };
    envv = {};

    gtk_widget_destroy(destination_chooser_dialog);
    vte_terminal_fork_command(terminal, "/usr/bin/wget", argv, envv, NULL, FALSE, FALSE, FALSE);

    gtk_container_add(GTK_CONTAINER(view_source_window), GTK_WIDGET(scrolled_window));

#ifdef DEBUG
    puts("Showing Download window...");
#endif

    gtk_widget_show_all(GTK_WIDGET(download_window));
}
