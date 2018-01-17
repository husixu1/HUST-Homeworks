CXX = g++
CXXFLAGS = -Wall -Werror -std=c++14 -O3
CXXFLAGS_DEBUG = -Wall -Werror -g -ggdb -std=c++14 -D_GLIBCXX_DEBUG

SRC = main.cpp

TARGET = alg4
TARGET_DEBUG = alg4_debug

.PHONY: release debug clean

release: ${SRC}
	$(CXX) ${CXXFLAGS} -o ${TARGET} ${SRC}

debug: ${SRC}
	$(CXX) ${CXXFLAGS_DEBUG} -o ${TARGET_DEBUG} ${SRC}

clean:
	rm -rf ${TARGET} ${TARGET_DEBUG}
