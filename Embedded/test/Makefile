EXENAME := test
EXESRCS := test.c

include ../common/rules.mk

all:
	adb push ./test /
	adb shell ./test
