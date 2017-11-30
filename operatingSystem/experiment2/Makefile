CC = gcc
CFLAGS = -Wall -Werror -g -static -std=c11
LDFLAGS = -lpthread
INCLUDE =
SOURCES = main.c sem.c
TARGET = experiment2.exe

.PHONY: all clean

all: ${SOURCES}
	$(CC) ${CFLAGS} -o ${TARGET} ${SOURCES} ${LDFLAGS}

clean:
	@rm -rf ${TARGET}
