CC = gcc
#CFLAGS = -Wall -Werror -g -static -std=c11 -DLOG_USE_COLOR
CFLAGS = -Wall -Werror -O2 -std=c11 -DLOG_USE_COLOR
LDFLAGS = -lpthread
INCLUDE =
SOURCES = main.c sem.c
TARGET = experiment3.exe

.PHONY: all clean

all: ${SOURCES}
	$(CC) ${CFLAGS} -o ${TARGET} ${SOURCES} ${LDFLAGS}

clean:
	@rm -rf ${TARGET}
