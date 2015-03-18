WebKit/GTK is still somewhat experimental and incomplete, and crashes happen from time to time. If you have the time, we greatly appreciate information on crashes. If you notice any crash while using Juniper, even if you suspect it is related to WebKit, please report it to our issue tracker. This allows us to make sure no Juniper-specific crashes get into the WebKit bug tracker, and also means we might fix the problem in WebKit sooner.

To compile a debug build of WebKit, please run the following inside the repository root:

```
WebKitTools/Scripts/set-webkit-configuration --debug
WebKitTools/Scripts/build-webkit --gtk
```

Then, copy the generated libraries over the non-debug ones, as root:

```
cp WebKitBuild/Debug/lib/* /usr/lib/
```

There is a commented-out CFLAGS line in Juniper's `build.sh` that enables `-ggdb` for debugging support. Comment out the existing CFLAGS line and uncomment this one, then run:

```
./build.sh
```

Run Juniper under GDB and supply us with as much information as possible about the crash.