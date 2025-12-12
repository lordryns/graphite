CC=clang 
TARGET=main.c log.c ini.c
OUTPUT=graphite 
CFLAGS=`pkg-config --cflags gtk4` `pkg-config --cflags gtk4-layer-shell-0` 
LFLAGS=`pkg-config --libs gtk4` `pkg-config --libs gtk4-layer-shell-0` 
OFLAGS=-Wall -Wextra

build:
	$(CC) $(TARGET) -o $(OUTPUT) $(CFLAGS) $(LFLAGS) $(OFLAGS)

clean:
	rm -rf $(OUTPUT) 

.PHONY: build clean
