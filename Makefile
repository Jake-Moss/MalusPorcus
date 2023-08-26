RAYLIBPATH = $(shell readlink -f raylib)
EMSDKPATH = $(shell readlink -f emsdk)

export PATH := $(EMSDKPATH):$(EMSDKPATH)/upstream/emscripten:$(EMSDKPATH)/node/16.20.0_64bit/bin:$(PATH)
export C_INCLUDE_PATH := $(RAYLIBPATH)/src:$(C_INCLUDE_PATH)
export LIBRARY_PATH := $(RAYLIBPATH)/src:$(RAYLIBPATH)/src/external:$(LIBRARY_PATH)
export LD_LIBRARY_PATH := $(RAYLIBPATH)/src:$(LD_LIBRARY_PATH)

.SUFFIXES:

TARGET=main

SRCS = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJS = $(patsubst %.c,%.o,$(SRCS))

P ?= DESKTOP
ifeq ($(P),WEB)
	PLATFORM = PLATFORM_WEB
else
	PLATFORM = PLATFORM_DESKTOP
endif

ifeq ($(PLATFORM),PLATFORM_WEB)
	CC=emcc
	LDFLAGS += --shell-file $(RAYLIBPATH)/src/shell.html -s USE_GLFW=3 -s ASYNCIFY -pthread $(RAYLIBPATH)/src/libraylib.a
	OUT=main.html
else
	CC=gcc
	OUT=main
endif

RAYLIB_LIBTYPE ?= STATIC

UNAME_S := $(shell uname -s)
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	ifeq ($(UNAME_S),Linux)
		CCFLAGS += -D$(PLATFORM)
		LDFLAGS += -lraylib -lGL -pthread -ldl -lrt -lX11
		RAYLIB_LIBTYPE = SHARED
	endif
	ifeq ($(UNAME_S),Darwin)
		LDFLAGS += $(shell pkg-config --libs --cflags raylib)
	endif
endif

CCFLAGS += -Wall -std=c99 -pthread -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -O2
LDFLAGS += -lm



.PHONY: raylib raylib-examples setup leaks run debug host emsdk
all: $(TARGET)

setup : raylib

debug: CCFLAGS += -g -O0 -DPHYSAC_DEBUG
debug: clean $(TARGET) clean

clean:
	rm -f *.o *.wasm *.html *.js $(OUT)

run : $(TARGET)
	./$(OUT) $(ARGS)

host :
	$(EMSDKPATH)/upstream/emscripten/emrun $(OUT)

leaks: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(OUT) $(ARGS)

raylib :
	cd $(RAYLIBPATH)/src && $(MAKE) clean
	cd $(RAYLIBPATH)/src && $(MAKE) PLATFORM=$(PLATFORM) RAYLIB_LIBTYPE=$(RAYLIB_LIBTYPE) CUSTOM_CFLAGS=-pthread

emsdk :
	cd $(EMSDKPATH) && ./emsdk install latest && ./emsdk activate latest

raylib-examples : raylib
	cd $(RAYLIBPATH)/examples && $(MAKE) PLATFORM=$(PLATFORM)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $(LDFLAGS) $^ -o $(OUT)

%.o: %.c %.h
	$(CC) -c $(CCFLAGS) $< -o $@
