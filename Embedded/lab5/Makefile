
EXENAME:=lab5
EXESRCS:=main.c

include ../common/rules.mk

all:
	adb push lab5 /
	adb push background.jpg /
	adb push ../font.ttc /data/local
	adb shell ./lab5
