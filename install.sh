#!/bin/sh

set -e
set -x

cp juniper /usr/bin/

mkdir -p /usr/share/juniper
cp juniper.glade /usr/share/juniper/
