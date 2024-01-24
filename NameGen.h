#include <codecvt>
#include <cstdio>
#include <cstdlib>
#include <ctgmath>
#include <random>
#include <regex>
#include <unordered_map>
#include <vector>

#pragma once

namespace NameGen {
	namespace {
		template <std::integral T, typename R>
		inline T randrange(T min, T max, R &rng) {
			if constexpr (std::is_unsigned_v<T>)
				if (max == 0)
					return 0;
			return std::uniform_int_distribution<T>(min, max - 1)(rng);
		}

		template <std::integral T, typename R>
		inline T randrange(T max, R &rng) {
			return randrange(T(0), max, rng);
		}
	}

	struct Phonemes {
		std::wstring C = L"ptkmnls", V = L"aeiou", S = L"s", F = L"mn", L = L"rl";
		std::wstring operator[](wchar_t) const;
	};

	struct Orthography {
		std::string name = "Default";
		std::unordered_map<wchar_t, std::wstring> map;
	};

	struct Set {
		std::string name;
		std::wstring set;
	};

	struct RegexSet {
		std::string name;
		std::vector<std::wregex> regexen;
	};

	extern std::vector<Set> consets;
	extern std::vector<Set> vowsets;
	extern std::vector<Set> lsets;
	extern std::vector<Set> ssets;
	extern std::vector<Set> fsets;
	extern std::vector<std::wstring> syllstructs;
	extern std::vector<RegexSet> ressets;
	extern std::vector<Orthography> corthsets;
	extern std::vector<Orthography> vorthsets;
	extern Orthography defaultOrthography;

	std::string convert(const std::wstring &);
	std::wstring join(const std::vector<std::wstring> &, const std::wstring &sep = {});

	template <template <typename...> class C, typename... CArgs, typename T, typename R>
	const T & choose(const C<T, CArgs...> &container, R &rng, size_t exponent = 1) {
		std::uniform_real_distribution distribution(0.0, 1.0);
		return container.at(floor(pow(distribution(rng), exponent) * container.size()));
	}

	template <typename C, typename R>
	C shuffled(C container, R &rng) {
		for (size_t i = container.size() - 1; 0 < i; --i)
			std::swap(container.at(i), container.at(randrange(i, rng)));
		return container;
	}

	template <typename T>
	std::basic_string<T> capitalize(std::basic_string<T> word) {
		if (!word.empty())
			word.at(0) = std::toupper(word.at(0));
		return word;
	}

	template <typename R = std::default_random_engine>
	class Language {
		public:
			Phonemes phonemes;
			std::wstring structure = L"CVC";
			size_t exponent = 2;
			std::vector<std::wregex> restricts;
			Orthography cortho, vortho;
			bool noortho = true, nomorph = true, nowordpool = true;
			size_t minsyll = 1, maxsyll = 1;
			std::unordered_map<std::string, std::vector<std::wstring>> morphemes, words;
			std::vector<std::wstring> names;
			std::wstring joiner = L" ";
			size_t maxchar = 12, minchar = 5;
			std::wstring genitive, definite;

			std::wstring spell(const std::wstring &syllable) {
				if (noortho)
					return syllable;

				std::wstring s;

				for (size_t i = 0; i < syllable.size(); ++i) {
					wchar_t c = syllable.at(i);
					if (cortho.map.count(c) != 0)
						s += cortho.map.at(c);
					else if (vortho.map.count(c) != 0)
						s += vortho.map.at(c);
					else if (defaultOrthography.map.count(c) != 0)
						s += defaultOrthography.map.at(c);
					else
						s += c;
				}

				return s;
			}

			std::wstring makeSyllable() {
				for (;;) {
					std::wstring syllable;
					for (size_t i = 0; i < structure.size(); ++i) {
						wchar_t ptype = structure.at(i);
						if (i + 1 != structure.size() && structure.at(i + 1) == L'?') {
							++i;
							if (rand() % 2 == 0)
								continue;
						}
						syllable += choose(phonemes[ptype], rng, exponent);
					}
					bool bad = false;
					for (size_t i = 0; i < restricts.size(); ++i)
						if (std::regex_match(syllable, restricts.at(i))) {
							bad = true;
							break;
						}
					if (bad)
						continue;
					return spell(syllable);
				}
			}

			std::wstring getMorpheme(const std::string &key = "") {
				if (nomorph)
					return makeSyllable();

				std::vector<std::wstring> vec;

				if (morphemes.count(key) != 0)
					vec = morphemes.at(key);

				size_t extras = key.empty()? 10 : 1;

				for (;;) {
					size_t n = randrange(size_t(0), vec.size() + extras, rng);

					if (n < vec.size())
						return vec.at(n);

					std::wstring morph = makeSyllable();
					bool bad = false;

					for (const std::pair<const std::string, std::vector<std::wstring>> &pair: morphemes) {
						if (std::find(pair.second.begin(), pair.second.end(), morph) != pair.second.end()) {
							bad = true;
							break;
						}
					}

					if (bad)
						continue;

					vec.push_back(morph);
					morphemes[key] = vec;
					return morph;
				}
			}

			std::wstring makeWord(const std::string &key) {
				size_t nsylls = randrange(minsyll, maxsyll + 1, rng);
				std::wstring w;
				std::vector<std::string> keys;
				keys.resize(nsylls);
				keys.at(randrange(nsylls, rng)) = key;

				for (size_t i = 0; i < nsylls; ++i)
					w += getMorpheme(keys.at(i));

				return w;
			}

			std::wstring getWord(const std::string &key = "") {
				std::vector<std::wstring> ws;

				if (words.count(key) != 0)
					ws = words.at(key);

				size_t extras = key.empty()? 3 : 2;

				for (;;) {
					size_t n = randrange(ws.size() + extras, rng);

					if (n < ws.size())
						return ws.at(n);

					std::wstring w = makeWord(key);
					bool bad = false;

					for (const std::pair<const std::string, std::vector<std::wstring>> &pair: words) {
						if (std::find(pair.second.begin(), pair.second.end(), w) != pair.second.end()) {
							bad = true;
							break;
						}
					}

					if (bad)
						continue;

					ws.push_back(w);
					words[key] = ws;
					return w;
				}
			}

			std::string makeName(const std::string &key = "") {
				if (genitive.empty())
					genitive = getMorpheme("of");
				if (definite.empty())
					definite = capitalize(getMorpheme("the"));
				for (;;) {
					std::wstring name;
					if (rand() % 2 == 0) {
						name = capitalize(getWord(key));
					} else {
						std::wstring w1 = capitalize(getWord(rand() % 10 < 6? key : ""));
						std::wstring w2 = capitalize(getWord(rand() % 10 < 6? key : ""));
						if (w1 == w2)
							continue;
						if (rand() % 2 == 1)
							name = join({w1, w2}, joiner);
						else
							name = join({w1, genitive, w2}, joiner);
					}
					if (rand() % 10 < 1)
						name = join({definite, name}, joiner);
					if (name.size() < minchar || maxchar < name.size())
						continue;
					bool used = false;
					for (size_t i = 0; i < names.size(); ++i) {
						std::wstring name2 = names[i];
						if (name.find(name2) != std::wstring::npos || name2.find(name) != std::wstring::npos) {
							used = true;
							break;
						}
					}
					if (used)
						continue;
					names.push_back(name);
					return convert(name);
				}
			}

			template <std::integral T>
			Language(T seed):
				rng(seed) {}

		private:
			R rng;
	};

	template <typename R>
	Language<R> makeOrthoLanguage(R &rng) {
		Language<R> language(rng());
		language.noortho = false;
		return language;
	}

	template <typename R>
	Language<R> makeRandomLanguage(R &rng) {
		Language<R> language(rng());
		language.noortho = language.nomorph = language.nowordpool = false;
		language.phonemes.C = shuffled(choose(consets, rng, 2).set, rng);
		language.phonemes.V = shuffled(choose(vowsets, rng, 2).set, rng);
		language.phonemes.L = shuffled(choose(lsets, rng, 2).set, rng);
		language.phonemes.S = shuffled(choose(ssets, rng, 2).set, rng);
		language.phonemes.F = shuffled(choose(fsets, rng, 2).set, rng);
		language.structure = choose(syllstructs, rng);
		language.restricts = ressets[2].regexen;
		language.cortho = choose(corthsets, rng, 2);
		language.vortho = choose(vorthsets, rng, 2);
		language.minsyll = randrange(1, 3, rng);
		if (language.structure.size() < 3)
			++language.minsyll;
		language.maxsyll = randrange(language.minsyll + 1, size_t(7), rng);
		language.joiner = std::wstring(1, choose(std::wstring(L"   -"), rng));
		return language;
	}
}
