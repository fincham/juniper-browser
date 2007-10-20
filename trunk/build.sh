#!/bin/sh
set -e
set -x

COMMON="-march=native -O2 -maccumulate-outgoing-args -s -pipe -Wall -Werror -pedantic -fPIC"
#COMMON="-march=native -ggdb -maccumulate-outgoing-args -pipe -Wall -Werror -pedantic -fPIC"

DEPENDENCIES="libglade-2.0 gtk+-2.0 gtksourceview-1.0 WebKitGtk"

CFLAGS="$COMMON $(pkg-config --cflags $DEPENDENCIES)"
LDFLAGS="$COMMON -Wl,--as-needed -Wl,--no-add-needed -Wl,--sort-common -Wl,-E -Wl,--gc-sections -Wl,--fatal-warnings -Wl,--relax -Wl,--enable-new-dtags"
LIBS="$(pkg-config --libs $DEPENDENCIES) -lyaml"

cd src

gcc $CFLAGS -c juniper.c -o ../juniper.o
gcc $CFLAGS -c juniper-fs.c -o ../juniper-fs.o
gcc $CFLAGS -c juniper-prefs.c -o ../juniper-prefs.o
gcc $CFLAGS -c juniper-bookmarks.c -o ../juniper-bookmarks.o
gcc $CFLAGS -c juniper-tabs.c -o ../juniper-tabs.o
gcc $CFLAGS -c juniper-events.c -o ../juniper-events.o
gcc $CFLAGS -c juniper-view-source.c -o ../juniper-view-source.o
gcc $CFLAGS -c juniper-ui.c -o ../juniper-ui.o

cd ..

# Build dynamically-linked Juniper executable
gcc $LDFLAGS -o juniper juniper.o juniper-tabs.o juniper-events.o juniper-fs.o juniper-prefs.o juniper-bookmarks.o juniper-view-source.o juniper-ui.o $LIBS

rm -f *.o

# build tools
cd tools

gcc -o gtk-key-snooper $(pkg-config --cflags gtk+-2.0 libglade-2.0) $(pkg-config --libs gtk+-2.0) -Wl,-E $(pkg-config --libs libglade-2.0) gtk-key-snooper.c

cd ..
