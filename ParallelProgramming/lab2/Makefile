CXX = g++
CXXFLAGS = -g
LDFLAGS = -lm -lpthread -lX11

SOURCES = main.cpp

TARGET = lab2

.PHONY: all clean

all : ${TARGET}

${TARGET} : ${SOURCES}
	$(CXX) $^ -o $@ ${CXXFLAGS} ${LDFLAGS}

clean:
	rm -rf *.png
	rm -rf ${TARGET}
