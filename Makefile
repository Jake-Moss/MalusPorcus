RAYLIBPATH = $(shell readlink -f raylib)

export PATH := $(TINYCCPATH):$(PATH)
export C_INCLUDE_PATH := $(RAYLIBPATH)/src:$(C_INCLUDE_PATH)
export LIBRARY_PATH := $$(RAYLIBPATH)/src/external:$(LIBRARY_PATH)
export LD_LIBRARY_PATH := $(RAYLIBPATH)/src:$(LD_LIBRARY_PATH)

CCFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -O2 -DPLATFORM_DESKTOP
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: raylib raylib-examples setup
raylib :
	cd $(RAYLIBPATH)/src && $(MAKE) PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED

raylib-examples : raylib
	# gsed -i -e 's/LDLIBS += -latomic/# LDLIBS += -latomic/' raylib/examples/Makefile
	cd $(RAYLIBPATH)/examples && $(MAKE) PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED

setup : raylib
