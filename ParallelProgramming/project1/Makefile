CXX = g++
CXXFLAGS = -g
LDFLAGS = -fopenmp

SOURCES = serial.cpp parallel.cpp

TARGET = project1_serial project1_parallel

.PHONY: all clean

all : ${TARGET}

project1_% : %.cpp
	$(CXX) $^ -o $@ ${CXXFLAGS} ${LDFLAGS}

clean:
	rm -rf ${TARGET}
	rm -rf solution.txt
