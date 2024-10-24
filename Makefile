CC=gcc
CXX=g++
MAKE=make
CMAKE=cmake
NPM=npm

UNAME_S := $(shell uname -s)

override CFLAGS+=-Wall -march=native -mtune=native -std=gnu89
override CFLAGS+=-Ilibs/webview/dist/usr/include
override CFLAGS+=-Ilibs/cJSON

ifeq ($(UNAME_S),Linux)
override CFLAGS+=$(shell pkg-config --cflags webkit2gtk-4.0 gtk+-3.0)
endif

CXXFLAGS=$(CFLAGS)

override LDFLAGS+=-Llibs/cJSON -lcjson
override LDFLAGS+=-Llibs/webview/dist/usr/lib -lwebview

ifeq ($(UNAME_S),Linux)
override LDFLAGS+=$(shell pkg-config --libs webkit2gtk-4.0 gtk+-3.0) -lstdc++
else ifeq ($(UNAME_S),Darwin)
override LDFLAGS+=-framework WebKit -ldl -lstdc++
endif

ifeq ($(UNAME_S),Linux)
DYNLIB_EXT = so
else ifeq ($(UNAME_S),Darwin)
DYNLIB_EXT = dylib
endif

SRC=$(wildcard src/**/*.c src/*.c)
OBJS=$(SRC:.c=.o)

OUTPUT=file-explorer

all: libs css $(OUTPUT)

libs: libs/webview/dist/usr/lib/libwebview.a libs/cJSON/libcjson.a

libs/webview/dist/usr/lib/libwebview.a:
	cd libs/webview && mkdir -p build dist
	cd libs/webview/build && $(CMAKE) -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..
	cd libs/webview/build && $(MAKE)
	cd libs/webview/build && $(MAKE) DESTDIR=../dist install
	test -d libs/webview/dist/usr/lib64 && mv -v libs/webview/dist/usr/lib{64,} || true
	cd libs/webview/dist/usr/lib && rm *.$(DYNLIB_EXT)

libs/cJSON/libcjson.a:
	cd libs/cJSON && $(MAKE)
	cd libs/cJSON && rm *.$(DYNLIB_EXT)*

css:
	mkdir -p src/resources/dist
	test -d node_modules || $(NPM) install
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
