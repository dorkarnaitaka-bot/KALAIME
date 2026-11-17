#include "kalaime/unicode/bengali.hpp"

#include <unicode/normalizer2.h>
#include <unicode/unistr.h>

namespace kalaime::unicode {

std::string normalize_nfkc(const std::string& text) {
    if (text.empty()) {
        return {};
    }

    UErrorCode status = U_ZERO_ERROR;
    const icu::Normalizer2* normalizer = icu::Normalizer2::getNFKCInstance(status);
    if (U_FAILURE(status) || normalizer == nullptr) {
        return text;
    }

    icu::UnicodeString source = icu::UnicodeString::fromUTF8(std::string(text));
    icu::UnicodeString normalized;
    normalizer->normalize(source, normalized, status);

    std::string converted;
    normalized.toUTF8String(converted);
    return converted;
}

std::vector<BengaliCharacterInfo> supported_bengali_inventory() {
    return {{"অ", "Bengali Letter A", "Lo", "a", "অ"},
            {"আ", "Bengali Letter AA", "Lo", "aa", "আ"},
            {"ই", "Bengali Letter I", "Lo", "i", "ই"},
            {"ঈ", "Bengali Letter II", "Lo", "ii", "ঈ"},
            {"উ", "Bengali Letter U", "Lo", "u", "উ"},
            {"ঊ", "Bengali Letter UU", "Lo", "uu", "ঊ"},
            {"ঋ", "Bengali Letter Vocalic R", "Lo", "ri", "ঋ"},
            {"এ", "Bengali Letter E", "Lo", "e", "এ"},
            {"এ", "Bengali Letter E", "Lo", "ei", "এ"},
            {"ও", "Bengali Letter O", "Lo", "o", "ও"},
            {"ক", "Bengali Letter Ka", "Lo", "k", "ক"},
            {"খ", "Bengali Letter Kha", "Lo", "kh", "খ"},
            {"গ", "Bengali Letter Ga", "Lo", "g", "গ"},
            {"ঘ", "Bengali Letter Gha", "Lo", "gh", "ঘ"},
            {"চ", "Bengali Letter Ca", "Lo", "c", "চ"},
            {"ছ", "Bengali Letter Cha", "Lo", "ch", "ছ"},
            {"জ", "Bengali Letter Ja", "Lo", "j", "জ"},
            {"ঝ", "Bengali Letter Jha", "Lo", "jh", "ঝ"},
            {"ট", "Bengali Letter Tta", "Lo", "tt", "ট"},
            {"ঠ", "Bengali Letter Ttha", "Lo", "tth", "ঠ"},
            {"ড", "Bengali Letter Dda", "Lo", "dd", "ড"},
            {"ঢ", "Bengali Letter Ddha", "Lo", "ddh", "ঢ"},
            {"ণ", "Bengali Letter Nna", "Lo", "nn", "ণ"},
            {"ত", "Bengali Letter Ta", "Lo", "t", "ত"},
            {"থ", "Bengali Letter Tha", "Lo", "th", "থ"},
            {"দ", "Bengali Letter Da", "Lo", "d", "দ"},
            {"ধ", "Bengali Letter Dha", "Lo", "dh", "ধ"},
            {"ন", "Bengali Letter Na", "Lo", "n", "ন"},
            {"প", "Bengali Letter Pa", "Lo", "p", "প"},
            {"ফ", "Bengali Letter Pha", "Lo", "ph", "ফ"},
            {"ব", "Bengali Letter Ba", "Lo", "b", "ব"},
            {"ভ", "Bengali Letter Bha", "Lo", "bh", "ভ"},
            {"ম", "Bengali Letter Ma", "Lo", "m", "ম"},
            {"য", "Bengali Letter Ya", "Lo", "y", "য"},
            {"র", "Bengali Letter Ra", "Lo", "r", "র"},
            {"ল", "Bengali Letter La", "Lo", "l", "ল"},
            {"শ", "Bengali Letter Sha", "Lo", "sh", "শ"},
            {"ষ", "Bengali Letter Ssa", "Lo", "shh", "ষ"},
            {"স", "Bengali Letter Sa", "Lo", "s", "স"},
            {"হ", "Bengali Letter Ha", "Lo", "h", "হ"},
            {"ঙ", "Bengali Letter Nga", "Lo", "ng", "ঙ"},
            {"ঞ", "Bengali Letter Nya", "Lo", "ny", "ঞ"},
            {"ং", "Bengali Sign Anusvara", "Mn", "ng", "ং"},
            {"ঃ", "Bengali Sign Visarga", "Mc", "h", "ঃ"},
            {"্", "Bengali Sign Virama", "Mn", "", "্"},
            {"া", "Bengali Vowel Sign Aa", "Mn", "a", "া"},
            {"ি", "Bengali Vowel Sign I", "Mn", "i", "ি"},
            {"ী", "Bengali Vowel Sign II", "Mn", "ii", "ী"},
            {"ু", "Bengali Vowel Sign U", "Mn", "u", "ু"},
            {"ূ", "Bengali Vowel Sign UU", "Mn", "uu", "ূ"},
            {"ৃ", "Bengali Vowel Sign Vocalic R", "Mn", "ri", "ৃ"},
            {"ে", "Bengali Vowel Sign E", "Mn", "e", "ে"},
            {"ৈ", "Bengali Vowel Sign AI", "Mn", "oi", "ৈ"},
            {"ো", "Bengali Vowel Sign O", "Mn", "o", "ো"},
            {"ৌ", "Bengali Vowel Sign AU", "Mn", "ou", "ৌ"},
            {"৳", "Bengali Rupee Sign", "Sc", "rs", "৳"},
            {"০", "Bengali Digit Zero", "Nd", "0", "০"},
            {"১", "Bengali Digit One", "Nd", "1", "১"},
            {"২", "Bengali Digit Two", "Nd", "2", "২"},
            {"৩", "Bengali Digit Three", "Nd", "3", "৩"},
            {"৪", "Bengali Digit Four", "Nd", "4", "৪"},
            {"৫", "Bengali Digit Five", "Nd", "5", "৫"},
            {"৬", "Bengali Digit Six", "Nd", "6", "৬"},
            {"৭", "Bengali Digit Seven", "Nd", "7", "৭"},
            {"৮", "Bengali Digit Eight", "Nd", "8", "৮"},
            {"৯", "Bengali Digit Nine", "Nd", "9", "৯"}};
}

bool is_bengali_codepoint(unsigned int code_point) {
    const auto begin = static_cast<unsigned int>(0x0980);
    const auto end = static_cast<unsigned int>(0x09FF);
    return code_point >= begin && code_point <= end;
}

std::vector<std::string> split_graphemes(const std::string& text) {
    std::vector<std::string> result;
    if (text.empty()) {
        return result;
    }

    std::string current;
    for (unsigned char ch : text) {
        current.push_back(static_cast<char>(ch));
        if ((ch & 0x80u) == 0) {
            result.push_back(current);
            current.clear();
        }
    }
    if (!current.empty()) {
        result.push_back(current);
    }
    return result;
}

}  // namespace kalaime::unicode
