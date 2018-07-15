CXX = mpic++
CXXFLAGS = -g
LDFLAGS = -lm -lX11

SOURCES = main.cpp

TARGET = lab4

.PHONY: all clean

all : ${TARGET}

${TARGET} : ${SOURCES}
	$(CXX) $^ -o $@ ${CXXFLAGS} ${LDFLAGS}

clean:
	rm -rf *.png
	rm -rf ${TARGET}
