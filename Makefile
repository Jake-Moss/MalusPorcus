RAYLIBPATH = $(shell readlink -f raylib)

export C_INCLUDE_PATH := $(RAYLIBPATH)/src:$(C_INCLUDE_PATH)
export LIBRARY_PATH := $(RAYLIBPATH)/src:$(RAYLIBPATH)/src/external:$(LIBRARY_PATH)
export LD_LIBRARY_PATH := $(RAYLIBPATH)/src:$(LD_LIBRARY_PATH)

CCFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -O2 -DPLATFORM_DESKTOP

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif
ifeq ($(UNAME_S),Darwin)
	LDFLAGS = $(pkg-config --libs --cflags raylib)
endif

.PHONY: raylib raylib-examples setup
raylib :
	cd $(RAYLIBPATH)/src && $(MAKE) PLATFORM=PLATFORM_DESKTOP

raylib-examples : raylib
	cd $(RAYLIBPATH)/examples && $(MAKE) PLATFORM=PLATFORM_DESKTOP

setup : raylib

core_2d_camera_platformer : core_2d_camera_platformer.c
	echo $(C_INCLUDE_PATH)
	gcc -o $@ $< $(CCFLAGS) $(LDFLAGS)

run:
	./core_2d_camera_platformer
