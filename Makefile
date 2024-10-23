CC=gcc
CXX=g++
MAKE=make
CMAKE=cmake
NPM=npm

CFLAGS=-Wall -march=native -mtune=native -std=gnu89
CFLAGS+=-Ilibs/webview/dist/usr/include
CFLAGS+=-Ilibs/cJSON
CFLAGS+=$(shell pkg-config --cflags webkitgtk-6.0)

CXXFLAGS=$(CFLAGS)

UNAME_S := $(shell uname -s)

LDFLAGS=
LDFLAGS+=-Llibs/cJSON -lcjson

ifeq ($(UNAME_S),Linux)
LDFLAGS+=-Llibs/webview/dist/usr/lib64 -lwebview
LDFLAGS+=$(shell pkg-config --libs webkitgtk-6.0) -lstdc++
endif

ifeq ($(UNAME_S),Darwin)
LDFLAGS+=-Llibs/webview/dist/usr/lib -lwebview
LDFLAGS+=-framework WebKit -ldl -lstdc++
endif

ifeq ($(UNAME_S),Linux)
LIB64 = lib64
DYNLIB_EXT = .so
else ifeq ($(UNAME_S),Darwin)
LIB64 = lib
DYNLIB_EXT = .dylib
endif

SRC=$(wildcard src/**/*.c src/*.c)
OBJS=$(SRC:.c=.o)

OUTPUT=file-explorer

all: libs css $(OUTPUT)

libs: libs/webview/dist/usr/$(LIB64)/libwebview.a libs/cJSON/libcjson.a

libs/webview/dist/usr/$(LIB64)/libwebview.a:
	cd libs/webview && mkdir -p build dist
	cd libs/webview/build && $(CMAKE) -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..
	cd libs/webview/build && $(MAKE)
	cd libs/webview/build && $(MAKE) DESTDIR=../dist install
	cd libs/webview/dist/usr/lib64 && rm *.$(DYNLIB_EXT)

libs/cJSON/libcjson.a:
	cd libs/cJSON && $(MAKE)
	cd libs/cJSON && rm *.$(DYNLIB_EXT)*

css:
	mkdir -p src/resources/dist
	$(NPM) run build

$(OUTPUT): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

run: all
	./$(OUTPUT)

format: $(SRC)
	bash -c 'find . -type f -name *.c | grep -v libs | while read -r filename; do clang-format -i $$filename; done'

clean: $(OUTPUT)
	rm $(OUTPUT) $(OBJS)
	rm -rf src/resources/dist

clean-libs: libs
	rm -rf libs/webview/build libs/webview/dist
	cd libs/cJSON && $(MAKE) clean
