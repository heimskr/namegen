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

	std::wstring join(const std::vector<std::wstring> &vector, const std::wstring &sep) {
		if (vector.empty())
			return {};

		std::wstring s = vector.at(0);

		for (size_t i = 1; i < vector.size(); ++i)
			s += sep + vector.at(i);

		return s;
	}
}

#ifdef NAMEGEN_INCLUDE_MAIN
int main() {
	for (size_t i = 0; i < 10; ++i) {
		std::random_device device;
		std::default_random_engine engine(device());
		auto random = NameGen::makeRandomLanguage(engine);
		for (size_t j = 0; j < 4; ++j)
			std::cout << random.makeName() << "\n";
		std::cout << std::string(20, '-') << "\n";
	}
}
#endif
