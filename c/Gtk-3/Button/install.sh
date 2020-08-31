#!/bin/sh

cd ./src

gcc Main.c -o ../bin/Main-arm $(pkg-config --cflags --libs gtk+-3.0)


