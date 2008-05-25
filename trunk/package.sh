#!/bin/sh

set -e
set -x

test debian-control || exit

REVISION=$(svn info | grep Revision | awk '{ print $2; }')

VERSION="0~svn$REVISION"
ARCH=$(uname -m)

mkdir -p tmp
cd tmp

mkdir DEBIAN
chmod 755 DEBIAN
cd DEBIAN

cat ../../debian-control | sed "s/VERSION/$VERSION/" | sed "s/ARCH/$ARCH/" > control

cd ..

mkdir -p usr/bin usr/share/juniper
cp ../juniper usr/bin/
cp ../juniper.glade usr/share/juniper

cd ..

dpkg-deb -b tmp juniper-$VERSION.deb
rm -rf tmp
