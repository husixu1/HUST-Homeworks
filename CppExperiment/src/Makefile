CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -static -static-libstdc++

# a brief name for U2015xxxxx_y.exe
.PHONY: all 1 2 3 4 5 6

# map target
all: 1 2 3 4 5 6
1: U201514898_1.exe
2: U201514898_2.exe
3: U201514898_3.exe
4: U201514898_4.exe
5: U201514898_5.exe
6: U201514898_6.exe

# build rule
%.exe: %.cpp
	$(CXX) ${CXXFLAGS} $< -o $@

# clean rule
clean:
	@rm -f U201514898_*.exe *.TXT *.txt
