RAYLIBPATH = $(shell readlink -f raylib)

export C_INCLUDE_PATH := $(RAYLIBPATH)/src:$(C_INCLUDE_PATH)
export LIBRARY_PATH := $(RAYLIBPATH)/src:$(RAYLIBPATH)/src/external:$(LIBRARY_PATH)
export LD_LIBRARY_PATH := $(RAYLIBPATH)/src:$(LD_LIBRARY_PATH)

CC=gcc

CCFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -O2
LDFLAGS = -lm

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CCFLAGS += -DPLATFORM_DESKTOP
	LDFLAGS += -lraylib -lGL -lpthread -ldl -lrt -lX11
endif
ifeq ($(UNAME_S),Darwin)
	LDFLAGS += $(shell pkg-config --libs --cflags raylib)
endif

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

TARGET=main
OUT=main


.PHONY: raylib raylib-examples setup leaks run debug
all: $(TARGET)

setup : raylib

debug: CCFLAGS += -g -O0 -DPHYSAC_DEBUG
debug: clean $(TARGET) clean

clean:
	rm -f *.o $(OUT)

run: $(TARGET)
	./$(OUT) $(ARGS)

leaks: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(OUT) $(ARGS)

raylib :
	cd $(RAYLIBPATH)/src && $(MAKE) PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED

raylib-examples : raylib
	cd $(RAYLIBPATH)/examples && $(MAKE) PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $(LDFLAGS) $^ -o $(OUT)

$(TARGET).o: $(TARGET).c
	$(CC) -c $(CCFLAGS) $< -o $@

%.o: %.c %.h
	$(CC) -c $(CCFLAGS) $< -o $@
