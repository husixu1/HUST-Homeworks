CXX = g++
CXXFLAGS = -Wall -Werror -std=c++98 -O3
CXXFLAGS_DEBUG = -Wall -Werror -g -ggdb -std=c++98 -D_GLIBCXX_DEBUG
# GOD I don't believe I'm using c++98. It's outdated. POJ should switch to c++11 or higher.

SRC = main.cpp

TARGET = alg5
TARGET_DEBUG = alg5_debug

.PHONY: release debug clean

release: ${SRC}
	$(CXX) ${CXXFLAGS} -o ${TARGET} ${SRC}

debug: ${SRC}
	$(CXX) ${CXXFLAGS_DEBUG} -o ${TARGET_DEBUG} ${SRC}

clean:
	rm -rf ${TARGET} ${TARGET_DEBUG}
