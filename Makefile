CXX ?= clang++

.PHONY: all test clean

all: main

test: main
	./main

object: NameGen.cpp NameGen.h
	$(CXX) -std=c++17 -c NameGen.cpp

main: NameGen.cpp NameGen.h
	$(CXX) -std=c++17 -DNAMEGEN_INCLUDE_MAIN NameGen.cpp -o main -lstdc++

clean:
	rm -f main
