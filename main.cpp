#include <stdlib.h>
#include <ctgmath>
#include <regex>
#include <unordered_map>
#include <vector>

#define INCLUDE_MAIN

size_t randrange(size_t low, size_t high);
size_t randrange(size_t high);

namespace NameGen {
	using wregex = std::basic_regex<wchar_t>;

	struct Phonemes {
		std::wstring C = L"ptkmnls", V = L"aeiou", S = L"s", F = L"mn", L = L"rl";
	};

	struct Orthography {
		std::string name = "Default";
		std::unordered_map<wchar_t, std::wstring> map;
	};

	struct Language {
		Phonemes phonemes;
		std::wstring structure = L"CVC";
		size_t exponent = 2;
		std::vector<wregex> restricts;
		Orthography cortho, vortho;
		bool noortho = true, nomorph = true, nowordpool = true;
		size_t minsyll = 1, maxsyll = 1;
		std::unordered_map<std::string, std::wstring> morphemes, words;
		std::vector<std::wstring> names;
		std::wstring joiner = L" ";
		size_t maxchar = 12, minchar = 5;
	};

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

	template <typename T>
	std::basic_string<T> capitalize(std::basic_string<T> word) {
		if (!word.empty())
			word[0] = std::toupper(word[0]);
		return word;
	}
}

#ifdef INCLUDE_MAIN
int main() {

}
#endif
