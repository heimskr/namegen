#include <stdlib.h>
#include <ctgmath>
#include <vector>

#define INCLUDE_MAIN

size_t randrange(size_t low, size_t high);
size_t randrange(size_t high);

template <typename T>
std::vector<T> shuffled(std::vector<T> vector) {
    for (size_t i = vector.size() - 1; 0 < i; --i)
		std::swap(vector[i], vector[randrange(i)]);
    return vector;
}

template <template <typename S> class C, typename T>
T & choose(const C<T> &container, size_t exponent = 1) {
    return container[floor(pow(static_cast<double>(rand()) / static_cast<double>(RAND_MAX), exponent) * container.size())];
}

size_t randrange(size_t low, size_t high) {
	return (rand() % (high - low)) + low;
}

size_t randrange(size_t high) {
	return randrange(0, high);
}

#ifdef INCLUDE_MAIN
int main() {

}
#endif
