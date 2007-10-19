#include "gtk/gtk.h"
#include "glade/glade.h"

int textview_key_press(GtkWidget * widget, GdkEventKey * event)
{
    printf("type=%i, send_event=%i, time=%i, state=%i, keyval=%i, length=%i, hardware_keycode=%i, group=%i, is_modifier=%i\n", event->type, event->send_event, event->time, event->state, event->keyval, event->length, event->hardware_keycode, event->group, event->is_modifier);
    return FALSE;
}

int main(int argc, char **argv)
{
    GladeXML * xml;
    GtkTextView * textview;

    gtk_init(&argc, &argv);
    xml = glade_xml_new("gtk-key-snooper.glade", NULL, NULL);
    glade_xml_signal_autoconnect(xml);

    textview = GTK_TEXT_VIEW(glade_xml_get_widget(xml, "textview"));

    gtk_widget_show_all(GTK_WIDGET(glade_xml_get_widget(xml, "window")));
    gtk_main();

    return 0;
}
