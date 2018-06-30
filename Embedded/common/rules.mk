
DIR := "../../tools/android-ndk-r8c"

ifneq ($(shell if [ -d "$(DIR)" ]; then echo true; fi), true)
$(error DIR=$(DIR) does not exit)
endif

DIR := $(shell cd $(DIR) && pwd)

CC:=$(DIR)/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86/bin/arm-linux-androideabi-gcc
CFLAGS:=--sysroot=$(DIR)/platforms/android-9/arch-arm -march=armv7-a -mfloat-abi=softfp -mfpu=neon
LDFLAGS:=--sysroot=$(DIR)/platforms/android-9/arch-arm -march=armv7-a -mfloat-abi=softfp -mfpu=neon

INCLUDE := -I../common/external/include
LIB := ../common/external/lib/libturbojpeg.a ../common/external/lib/libfreetype.a ../common/external/lib/libpng12.a -lz -lm

EXESRCS := ../common/graphic.c ../common/touch.c ../common/external.c $(EXESRCS)
EXEOBJS := $(patsubst %.c, %.o, $(EXESRCS))

$(EXENAME): $(EXEOBJS)
	$(CC) $(LDFLAGS) -o $(EXENAME) $(EXEOBJS) $(LIB)

clean:
	rm -f $(EXENAME) $(EXEOBJS)

%.o: %.c ../common/common.h
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

