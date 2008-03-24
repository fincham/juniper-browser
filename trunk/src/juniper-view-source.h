#ifndef JUNIPER_VIEW_SOURCE
#define JUNIPER_VIEW_SOURCE

#include "gtk/gtk.h"
#include "gtksourceview/gtksourceview.h"

/**
 * Display a view source window for the source of the current tab.
 */
void juniper_view_source_current_tab();

/**
 * Update the status bar when the caret is moved in the gtksourceview.
 */
void juniper_view_source_mark_set(GtkSourceView *source_view, GtkTextIter *location, GtkTextMark *mark, GtkStatusbar *status_bar);

#endif
