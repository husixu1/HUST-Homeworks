CC = gcc
CFLAGS = -Wall -Werror -g -static -std=c11
INCLUDE =
SOURCES = main.c
TARGET = experiment1.exe

.PHONY: all clean

all: ${SOURCES}
	$(CC) ${CFLAGS} -o ${TARGET} ${SOURCES}

clean:
	@rm -rf ${TARGET}
