CC = gcc
CFLAGS = -Wall -Werror -g -ggdb -std=c11
LDFLAGS = -lpthread -lbsd
INCLUDE =
SOURCES = main.c
TARGET = experiment4.exe

RELEASEFLAGS = -Wall -Werror -O3 -Ofast -std=c11

.PHONY: all clean

all: ${SOURCES}
	$(CC) ${CFLAGS} -o ${TARGET} ${SOURCES} ${LDFLAGS}

release: ${SOURCES}
	$(CC) ${RELEASEFLAGS} -o ${TARGET} ${SOURCES} ${LDFLAGS}
clean:
	@rm -rf ${TARGET}

