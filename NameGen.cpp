#include <iostream>

#include "NameGen.h"

namespace NameGen {
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

	std::vector<Set> consets = {
		{"Minimal",              L"ptkmnls"},
		{"English-ish",          L"ptkbdgmnlrsʃzʒʧ"},
		{"Pirahã (very simple)", L"ptkmnh"},
		{"Hawaiian-ish",         L"hklmnpwʔ"},
		{"Greenlandic-ish",      L"ptkqvsgrmnŋlj"},
		{"Arabic-ish",           L"tksʃdbqɣxmnlrwj"},
		{"Arabic-lite",          L"tkdgmnsʃ"},
		{"English-lite",         L"ptkbdgmnszʒʧhjw"}
	};

	std::vector<Set> vowsets = {
		{"Standard 5-vowel",  L"aeiou"},
		{"3-vowel a i u",     L"aiu"},
		{"Extra A E I",       L"aeiouAEI"},
		{"Extra U",           L"aeiouU"},
		{"5-vowel a i u A I", L"aiuAI"},
		{"3-vowel e o u",     L"eou"},
		{"Extra A O U",       L"aeiouAOU"}
	};

	std::vector<Set> lsets = {
		{"r l",     L"rl"},
		{"Just r",  L"r"},
		{"Just l",  L"l"},
		{"w j",     L"wj"},
		{"r l w j", L"rlwj"}
	};

	std::vector<Set> ssets = {
		{"Just s", L"s"},
		{"s ʃ",    L"sʃ"},
		{"s ʃ f",  L"sʃf"}
	};

	std::vector<Set> fsets = {
		{"m n",     L"mn"},
		{"s k",     L"sk"},
		{"m n ŋ",   L"mnŋ"},
		{"s ʃ z ʒ", L"sʃzʒ"}
	};

	std::vector<std::wstring> syllstructs = {
		L"CVC", L"CVV?C", L"CVVC?", L"CVC?", L"CV", L"VC", L"CVF", L"C?VC", L"CVF?", L"CL?VC", L"CL?VF", L"S?CVC",
		L"S?CVF", L"S?CVC?", L"C?VF", L"C?VC?", L"C?VF?", L"C?L?VC", L"VC", L"CVL?C?", L"C?VL?C", L"C?VLC?"
	};

	std::vector<RegexSet> ressets = {
		{"None", {}},
		{"Double sounds", {std::wregex(L".*(.)\\1.*")}},
		{"Doubles and hard clusters", {
			std::wregex(L".*[sʃf][sʃ].*"),
			std::wregex(L".*(.)\\1.*"),
			std::wregex(L".*[rl][rl].*")
		}}
	};

	std::vector<Orthography> corthsets = {
		{"Default", {}},
		{"Slavic", {
			{L'ʃ', L"š"},
			{L'ʒ', L"ž"},
			{L'ʧ', L"č"},
			{L'ʤ', L"ǧ"},
			{L'j', L"j"},
		}},
		{"German", {
			{L'ʃ', L"sch"},
			{L'ʒ', L"zh"},
			{L'ʧ', L"tsch"},
			{L'ʤ', L"dz"},
			{L'j', L"j"},
			{L'x', L"ch"},
		}},
		{"French", {
			{L'ʃ', L"ch"},
			{L'ʒ', L"j"},
			{L'ʧ', L"tch"},
			{L'ʤ', L"dj"},
			{L'x', L"kh"},
		}},
		{"Chinese (pinyin)", {
			{L'ʃ', L"x"},
			{L'ʧ', L"q"},
			{L'ʤ', L"j"},
		}},
	};

	std::vector<Orthography> vorthsets = {
		{"Ácutes", {}},
		{"Ümlauts", {
			{L'A', L"ä"},
			{L'E', L"ë"},
			{L'I', L"ï"},
			{L'O', L"ö"},
			{L'U', L"ü"},
		}},
		{"Welsh", {
			{L'A', L"â"},
			{L'E', L"ê"},
			{L'I', L"y"},
			{L'O', L"ô"},
			{L'U', L"w"},
		}},
		{"Diphthongs", {
			{L'A', L"au"},
			{L'E', L"ei"},
			{L'I', L"ie"},
			{L'O', L"ou"},
			{L'U', L"oo"},
		}},
		{"Doubles", {
			{L'A', L"aa"},
			{L'E', L"ee"},
			{L'I', L"ii"},
			{L'O', L"oo"},
			{L'U', L"uu"},
		}},
	};

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

	std::string convert(const std::wstring &wstr) {
		return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
	}

	size_t randrange(size_t low, size_t high) {
		return (rand() % (high - low)) + low;
	}

	size_t randrange(size_t high) {
		return randrange(0, high);
	}

	std::wstring join(const std::vector<std::wstring> &vector, const std::wstring &sep) {
		if (vector.empty())
			return {};
		std::wstring s = vector.at(0);
		for (size_t i = 1; i < vector.size(); ++i)
			s += sep + vector.at(i);
		return s;
	}

	std::wstring spell(const Language &language, const std::wstring &syllable) {
		if (language.noortho)
			return syllable;
		std::wstring s;
		for (size_t i = 0; i < syllable.size(); ++i) {
			wchar_t c = syllable.at(i);
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
				wchar_t ptype = language.structure.at(i);
				if (i + 1 != language.structure.size() && language.structure.at(i + 1) == L'?') {
					++i;
					if (rand() % 2 == 0)
						continue;
				}
				syllable += choose(language.phonemes[ptype], language.exponent);
			}
			bool bad = false;
			for (size_t i = 0; i < language.restricts.size(); ++i)
				if (std::regex_match(syllable, language.restricts.at(i))) {
					bad = true;
					break;
				}
			if (bad)
				continue;
			return spell(language, syllable);
		}
	}

	std::wstring getMorpheme(Language &language, const std::string &key) {
		if (language.nomorph)
			return makeSyllable(language);
		std::vector<std::wstring> vec;
		if (language.morphemes.count(key) != 0)
			vec = language.morphemes.at(key);
		size_t extras = key.empty()? 10 : 1;
		for (;;) {
			size_t n = randrange(vec.size() + extras);
			if (n < vec.size())
				return vec.at(n);
			std::wstring morph = makeSyllable(language);
			bool bad = false;
			for (const std::pair<const std::string, std::vector<std::wstring>> &pair: language.morphemes)
				if (std::find(pair.second.begin(), pair.second.end(), morph) != pair.second.end()) {
					bad = true;
					break;
				}
			if (bad)
				continue;
			vec.push_back(morph);
			language.morphemes[key] = vec;
			return morph;
		}
	}

	std::wstring makeWord(Language &language, const std::string &key) {
		size_t nsylls = randrange(language.minsyll, language.maxsyll + 1);
		std::wstring w;
		std::vector<std::string> keys;
		keys.resize(nsylls);
		keys.at(randrange(nsylls)) = key;
		for (size_t i = 0; i < nsylls; ++i)
			w += getMorpheme(language, keys.at(i));
		return w;
	}

	std::wstring getWord(Language &language, const std::string &key) {
		std::vector<std::wstring> ws;
		if (language.words.count(key) != 0)
			ws = language.words.at(key);
		size_t extras = key.empty()? 3 : 2;
		for (;;) {
			size_t n = randrange(ws.size() + extras);
			if (n < ws.size())
				return ws.at(n);
			std::wstring w = makeWord(language, key);
			bool bad = false;
			for (const std::pair<const std::string, std::vector<std::wstring>> &pair: language.words)
				if (std::find(pair.second.begin(), pair.second.end(), w) != pair.second.end()) {
					bad = true;
					break;
				}
			if (bad)
				continue;
			ws.push_back(w);
			language.words[key] = ws;
			return w;
		}
	}

	std::string makeName(Language &language, const std::string &key) {
		if (language.genitive.empty())
			language.genitive = getMorpheme(language, "of");
		if (language.definite.empty())
			language.definite = capitalize(getMorpheme(language, "the"));
		for (;;) {
			std::wstring name;
			if (rand() % 2 == 0) {
				name = capitalize(getWord(language, key));
			} else {
				std::wstring w1 = capitalize(getWord(language, rand() % 10 < 6? key : ""));
				std::wstring w2 = capitalize(getWord(language, rand() % 10 < 6? key : ""));
				if (w1 == w2)
					continue;
				if (rand() % 2 == 1)
					name = join({w1, w2}, language.joiner);
				else
					name = join({w1, language.genitive, w2}, language.joiner);
			}
			if (rand() % 10 < 1)
				name = join({language.definite, name}, language.joiner);
			if (name.size() < language.minchar || language.maxchar < name.size())
				continue;
			bool used = false;
			for (size_t i = 0; i < language.names.size(); ++i) {
				std::wstring name2 = language.names[i];
				if (name.find(name2) != std::wstring::npos || name2.find(name) != std::wstring::npos) {
					used = true;
					break;
				}
			}
			if (used)
				continue;
			language.names.push_back(name);
			return convert(name);
		}
	}

	Language makeOrthoLanguage() {
		Language language;
		language.noortho = false;
		return language;
	}

	Language makeRandomLanguage() {
		Language language;
		language.noortho = language.nomorph = language.nowordpool = false;
		language.phonemes.C = shuffled(choose(consets, 2).set);
		language.phonemes.V = shuffled(choose(vowsets, 2).set);
		language.phonemes.L = shuffled(choose(lsets, 2).set);
		language.phonemes.S = shuffled(choose(ssets, 2).set);
		language.phonemes.F = shuffled(choose(fsets, 2).set);
		language.structure = choose(syllstructs);
		language.restricts = ressets[2].regexen;
		language.cortho = choose(corthsets, 2);
		language.vortho = choose(vorthsets, 2);
		language.minsyll = randrange(1, 3);
		if (language.structure.size() < 3)
			++language.minsyll;
		language.maxsyll = randrange(language.minsyll + 1, 7);
		language.joiner = std::wstring(1, choose(std::wstring(L"   -")));
		return language;
	}
}

#ifdef NAMEGEN_INCLUDE_MAIN
int main() {
	srand(time(nullptr));
	for (size_t i = 0; i < 10; ++i) {
		NameGen::Language random = NameGen::makeRandomLanguage();
		for (size_t j = 0; j < 4; ++j)
			std::cout << NameGen::makeName(random) << "\n";
		std::cout << std::string(20, '-') << "\n";
	}
}
#endif
