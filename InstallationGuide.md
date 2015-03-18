# Dependencies #

The main dependency is WebKit/GTK. Installation instructions for this are provided below.

We also depend on the following packages, all of which should be available in your distribution repositories. On most Gnome systems, they will already be installed, though you might need to install the -dev variants on binary distributions (like Ubuntu).

  * libglade-2.0
  * gtk+-2.0
  * gtksourceview-1.0
  * libxml2

# WebKit #

You'll need to install the WebKit/GTK library first. This is pretty straightforward.

Download the latest six-hourly snapshot of the WebKit SVN repository (this is much faster than checking it out via SVN as the repository is huge) and extract it:

```
wget http://live.nightly.webkit.org/files/WebKit-SVN-source.tar.bz2
tar xjf WebKit-SVN-source.tar.bz2
```

Enter the created directory and build the WebKit source (this will take a little while depending on your system specs):

```
cd WebKit
WebKitTools/Scripts/build-webkit --gtk
```

Install the WebKit libraries and header files:

```
cd WebKitBuild/Release
sudo make install
cd -
```

If you later want to update the repository before building again, run:

```
WebKit/WebKitTools/Scripts/update-webkit
```

# Juniper #

Once you have WebKit libraries and headers installed, building Juniper is simple. Just check out the source and then run our build script from within the checked out source directory:

```
svn co http://juniper-browser.googlecode.com/svn/trunk/ juniper
cd juniper
./build.sh
```

To build with debugging messages enabled on stdout, use:

```
./build.sh -DDEBUG
```

After building, install Juniper with:

```
sudo ./install.sh
```

You can then run 'juniper' to launch the browser.