CXX = g++
CXXFLAGS = -Wall -Werror -std=c++17 -O3
CXXFLAGS_DEBUG = -Wall -Werror -g -ggdb -std=c++17 -D_GLIBCXX_DEBUG

SRC = main.cpp

TARGET = alg7
TARGET_DEBUG = alg7_debug

.PHONY: release debug clean

release: ${SRC}
	$(CXX) ${CXXFLAGS} -o ${TARGET} ${SRC}

debug: ${SRC}
	$(CXX) ${CXXFLAGS_DEBUG} -o ${TARGET_DEBUG} ${SRC}

clean:
	rm -rf ${TARGET} ${TARGET_DEBUG}
