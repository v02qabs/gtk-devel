#!/bin/sh
gcc Main.c -o Main.arm $(pkg-config --cflags --libs gtk+-3.0) $(pkg-config --cflags --libs webkit2gtk-4.0) -I /usr/include/wekitgtk-4.0/

