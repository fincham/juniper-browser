Juniper is a lightweight, WebKit-based browser for GTK+. It is intended to eventually run on any platform GTK+ runs on, but currently has only been tested on Linux.

Juniper's main goals are:
  * Code correctness and simplicity
  * Minimal browser core with all extra features implemented as plugins which can be individually disabled
  * High performance and low memory footprint
  * Excellent web developer support (likely implemented via plugins, but shipped with the browser)

Since Juniper uses WebKit for document parsing, script execution and rendering, it inherits many great features of WebKit, including:
  * Best-of-breed rendering standards compliance
  * Excellent CSS support, including many CSS3 features
  * Modern, fast JavaScript engine
  * HTML5 client-side database storage
  * Client-side XSLT and XPath support

An InstallationGuide is available. No releases have been made of Juniper yet, so installation involves checking out the Subversion repository. Releases will be made once a stable version of WebKit/GTK is available.