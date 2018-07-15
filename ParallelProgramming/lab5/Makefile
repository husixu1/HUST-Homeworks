CXX = nvcc
CXXFLAGS = -g -std=c++14
LDFLAGS = -lm -lX11 -lcuda

SOURCES = main.cu

TARGET = lab5

.PHONY: all clean

all : ${TARGET}

${TARGET} : ${SOURCES}
	$(CXX) $^ -o $@ ${CXXFLAGS} ${LDFLAGS}

clean:
	rm -rf *.png
	rm -rf ${TARGET}
