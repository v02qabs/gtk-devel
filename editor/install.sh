#!/usr/bin/sh


gcc main.c -o ./aarch/editor $(pkg-config --cflags --libs gtk+-3.0)
