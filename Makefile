CC ?= clang++

all: main

object: main.cpp
	$(CC) -std=c++17 main.cpp

main: main.cpp
	$(CC) -std=c++17 main.cpp -o main

clean:
	rm -f main
