CXX = g++
CXXFLAGS = -g
LDFLAGS = -lm -lX11 -fopenmp

SOURCES = main.cpp

TARGET = lab3

.PHONY: all clean

all : ${TARGET}

${TARGET} : ${SOURCES}
	$(CXX) $^ -o $@ ${CXXFLAGS} ${LDFLAGS}

clean:
	rm -rf *.png
	rm -rf ${TARGET}
