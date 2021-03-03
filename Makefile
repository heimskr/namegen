CXX ?= clang++

all: main

object: main.cpp
	$(CXX) -std=c++17 main.cpp -lstdc++

main: main.cpp
	$(CXX) -std=c++17 main.cpp -o main -lstdc++

clean:
	rm -f main
