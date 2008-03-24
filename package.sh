#!/bin/sh

set -e
set -x

REVISION=$(svn info | grep Revision | awk '{ print $2; }')
ARCH=$(uname -m)

mkdir -p tmp
cd tmp

mkdir DEBIAN
cd DEBIAN

cat ../../debian-control | sed "s/REVISION/$REVISION/" | sed "s/ARCH/$ARCH/" > control

cd ..

mkdir -p usr/bin usr/share/juniper
cp ../juniper usr/bin/
cp ../juniper.glade usr/share/juniper

cd ..

dpkg-deb -b tmp juniper-0.$REVISION.deb
rm -rf tmp
