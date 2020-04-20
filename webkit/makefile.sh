gcc ./src/$1 -o ./bin/$2 -w $(pkg-config --cflags --libs gtk+-3.0) $(pkg-config --cflags --libs webkit2gtk-4.0)

