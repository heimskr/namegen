#include <stdlib.h>
#include <ctgmath>
#include <regex>
#include <unordered_map>
#include <vector>

#define INCLUDE_MAIN

size_t randrange(size_t low, size_t high);
size_t randrange(size_t high);

namespace NameGen {
	struct Phonemes {
		std::wstring C = L"ptkmnls", V = L"aeiou", S = L"s", F = L"mn", L = L"rl";
		std::wstring operator[](wchar_t) const;
	};

	struct Orthography {
		std::string name = "Default";
		std::unordered_map<wchar_t, std::wstring> map;
	};

	struct Language {
		Phonemes phonemes;
		std::wstring structure = L"CVC";
		size_t exponent = 2;
		std::vector<std::wregex> restricts;
		Orthography cortho, vortho;
		bool noortho = true, nomorph = true, nowordpool = true;
		size_t minsyll = 1, maxsyll = 1;
		std::unordered_map<std::string, std::wstring> morphemes, words;
		std::vector<std::wstring> names;
		std::wstring joiner = L" ";
		size_t maxchar = 12, minchar = 5;
	};

	Orthography defaultOrthography = {"Default", {
		{L'ʃ', L"sh"},
		{L'ʒ', L"zh"},
		{L'ʧ', L"ch"},
		{L'ʤ', L"j"},
		{L'ŋ', L"ng"},
		{L'j', L"y"},
		{L'x', L"kh"},
		{L'ɣ', L"gh"},
		{L'ʔ', L"‘"},
		{L'A', L"á"},
		{L'E', L"é"},
		{L'I', L"í"},
		{L'O', L"ó"},
		{L'U', L"ú"},
	}};

	std::wstring Phonemes::operator[](wchar_t type) const {
		switch (type) {
			case L'C': return C;
			case L'V': return V;
			case L'S': return S;
			case L'F': return F;
			case L'L': return L;
			default: throw std::invalid_argument("Invalid argument to Phonemes::operator[]");
		}
	}

	template <typename T>
	std::vector<T> shuffled(std::vector<T> vector) {
		for (size_t i = vector.size() - 1; 0 < i; --i)
			std::swap(vector[i], vector[randrange(i)]);
		return vector;
	}

	template <template <typename...> class C, typename... TArgs, typename T>
	const T & choose(const C<T, TArgs...> &container, size_t exponent = 1) {
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

	std::wstring spell(const Language &language, std::wstring syllable) {
		if (language.noortho)
			return syllable;
		std::wstring s;
		for (size_t i = 0; i < language.structure.size(); ++i) {
			wchar_t c = syllable[i];
			if (language.cortho.map.count(c) != 0)
				s += language.cortho.map.at(c);
			else if (language.vortho.map.count(c) != 0)
				s += language.vortho.map.at(c);
			else if (defaultOrthography.map.count(c) != 0)
				s += defaultOrthography.map.at(c);
			else
				s += c;
		}
		return s;
	}

	std::wstring makeSyllable(const Language &language) {
		for (;;) {
			std::wstring syllable;
			for (size_t i = 0; i < language.structure.size(); ++i) {
				wchar_t ptype = language.structure[i];
				if (language.structure[i + 1] == L'?') {
					++i;
					if (rand() % 2 == 0)
						continue;
				}
				syllable += choose(language.phonemes[ptype], language.exponent);
			}
			bool bad = false;
			for (size_t i = 0; i < language.restricts.size(); ++i)
				if (std::regex_match(syllable, language.restricts[i])) {
					bad = true;
					break;
				}
			if (bad)
				continue;
			return spell(language, syllable);
		}
	}

	std::wstring getMorpheme(Language &language, const std::string &key = "") {
		if (language.nomorph)
			return makeSyllable(language);
		std::wstring list;
		if (language.morphemes.count(key) != 0)
			list = language.morphemes.at(key);
		size_t extras = key.empty()? 10 : 1;
		for (;;) {
			size_t n = randrange(list.size() + extras);
			if (n < list.size())
				return std::wstring(1, list[n]);
			std::wstring morph = makeSyllable(language);
			bool bad = false;
			for (const std::pair<const std::string, std::wstring> &pair: language.morphemes)
				if (pair.second.find(morph) != std::wstring::npos) {
					bad = true;
					break;
				}
			if (bad)
				continue;
			list += morph;
			language.morphemes[key] = list;
			return morph;
		}
	}

	std::wstring makeWord(Language &language, const std::string &key) {
		size_t nsylls = randrange(language.minsyll, language.maxsyll + 1);
		std::wstring w;
		std::vector<std::string> keys;
		keys.resize(nsylls);
		keys[randrange(nsylls)] = key;
		for (size_t i = 0; i < nsylls; ++i)
			w += getMorpheme(language, keys[i]);
		return w;
	}

	std::wstring getWord(Language &language, const std::string &key = "") {
		std::wstring ws = language.words.count(key) == 0? L"" : language.words.at(key);
		size_t extras = key.empty()? 3 : 2;
		for (;;) {
			size_t n = randrange(ws.size() + extras);
			if (n < ws.size())
				return std::wstring(1, ws[n]);
			std::wstring w = makeWord(language, key);
			bool bad = false;
			for (const std::pair<const std::string, std::wstring> &pair: language.words)
				if (pair.second.find(w) != std::wstring::npos) {
					bad = true;
					break;
				}
			if (bad)
				continue;
			ws += w;
			language.words[key] = ws;
			return w;
		}
	}
}

#ifdef INCLUDE_MAIN
int main() {

}
#endif
