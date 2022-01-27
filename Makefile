print-%  : ; @echo $* = $($*)

UNAME_S = $(shell uname -s)

CC = g++
EMC = emcc

RAYLIB = libs/raylib/src
RAYLIB_BUILD = ../../../build/raylib

EMSDK_EXEC = libs/emsdk/emsdk
EMSDK_PATH = libs/emsdk

INCFLAGS += -Iinclude
INCFLAGS += -I${RAYLIB}

CCFLAGS  = -std=c++11 -O2 -g -Wall -Wextra -Wpedantic
CCFLAGS += -Wno-unused-parameter
CCFLAGS += $(INCFLAGS)

LDFLAGS  = -lm
LDFLAGS += -ldl
LDFLAGS += -lraylib
LDFLAGS += $(INCFLAGS)
LDFLAGS +=  -lGL

EMCFLAGS = -s USE_GLFW=3 -s ASYNCIFY --preload-file resources

SRC = src/*.cpp

LDFLAGS += -lstdc++

WEB_OUT = osus.html osus.wasm osus.js osus.data

build_libs:
	$(EMSDK_EXEC) install latest
	$(EMSDK_EXEC) activate latest

	mkdir -p build/raylib/desktop
	
	cd $(RAYLIB); make PLATFORM=PLATFORM_DESKTOP; rm *.o; mv libraylib.a $(RAYLIB_BUILD)/desktop

	mkdir -p build/raylib/web
	source "$(EMSDK_PATH)/emsdk_env.sh";
	cd $(RAYLIB); emcc -c rcore.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2;emcc -c rshapes.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2;emcc -c rtextures.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2;emcc -c rtext.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2;emcc -c rmodels.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2;emcc -c utils.c -Os -Wall -DPLATFORM_WEB;emcc -c raudio.c -Os -Wall -DPLATFORM_WEB; emar rcs libraylib.a rcore.o rshapes.o rtextures.o rtext.o rmodels.o utils.o raudio.o; rm *.o; mv libraylib.a $(RAYLIB_BUILD)/web

build_desktop:
#	$(CC) $(SRC) $(CCFLAGS) -Lbuild/raylib/desktop/ $(LDFLAGS)
	mkdir -p build/desktop/
	cd build/desktop; cmake ../..; make

build_web:
	source "$(EMSDK_PATH)/emsdk_env.sh"; $(EMC) -o osus.html $(SRC) $(CCFLAGS) -Lbuild/raylib/web/ $(LDFLAGS) $(EMCFLAGS)
	mkdir -p build/web/
	mv $(WEB_OUT) build/web/