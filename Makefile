CXX ?= clang++

.PHONY: all test clean

all: main

test: main
	./main

object: main.cpp
	$(CXX) -std=c++17 main.cpp -lstdc++

main: main.cpp
	$(CXX) -std=c++17 main.cpp -o main -lstdc++

clean:
	rm -f main
