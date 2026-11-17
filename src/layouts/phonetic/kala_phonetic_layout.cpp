#include "kalaime/layouts/phonetic/kala_phonetic_layout.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace kalaime::layouts {
namespace {

std::string to_lower(std::string input) {
    std::transform(input.begin(), input.end(), input.begin(),
                   [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return input;
}

std::string fallback_transliterate(const std::string& input) {
    static const std::vector<std::pair<std::string, std::string>> replacements = {
        {"sh", "শ"}, {"ss", "ষ"},  {"kh", "খ"},  {"gh", "ঘ"}, {"ch", "চ"}, {"jh", "ঝ"}, {"th", "থ"},
        {"dh", "ধ"}, {"ph", "ফ"},  {"bh", "ভ"},  {"ng", "ঙ"}, {"ny", "ঞ"}, {"tt", "ট"}, {"dd", "ড"},
        {"tt", "ট"}, {"thh", "ঠ"}, {"ddh", "ঢ"}, {"nn", "ণ"}, {"a", "অ"},  {"aa", "আ"}, {"i", "ই"},
        {"ii", "ঈ"}, {"u", "উ"},   {"uu", "ঊ"},  {"ri", "ঋ"}, {"e", "এ"},  {"o", "ও"},  {"ou", "ঔ"},
        {"k", "ক"},  {"g", "গ"},   {"j", "জ"},   {"t", "ত"},  {"d", "দ"},  {"n", "ন"},  {"p", "প"},
        {"b", "ব"},  {"m", "ম"},   {"y", "য"},   {"r", "র"},  {"l", "ল"},  {"s", "স"},  {"h", "হ"},
        {"q", "ক"},  {"w", "ও"},   {"x", "ক্স"}};

    std::string output;
    std::string remaining = input;
    while (!remaining.empty()) {
        bool matched = false;
        for (const auto& [token, value] : replacements) {
            if (remaining.rfind(token, 0) == 0) {
                output += value;
                remaining.erase(0, token.size());
                matched = true;
                break;
            }
        }
        if (!matched) {
            output += remaining.front();
            remaining.erase(0, 1);
        }
    }
    return output;
}

std::map<std::string, std::string> make_special_cases() {
    return {
        {"ami", "আমি"},     {"bangla", "বাংলা"},   {"bangladesh", "বাংলাদেশ"},
        {"bhasha", "ভাষা"}, {"shikkha", "শিক্ষা"},  {"bhalobasha", "ভালোবাসা"},
        {"kala", "কলা"},    {"kalaime", "কালাইম"}, {"bangla", "বাংলা"},
        {"bhasha", "ভাষা"}, {"ami", "আমি"},        {"shob", "সব"},
        {"dhaka", "ঢাকা"},  {"nodi", "নদী"},       {"sonkha", "সংখ্যা"},
    };
}

}  // namespace

std::string KalaPhoneticLayout::transliterate(const std::string& input) const {
    if (input.empty()) {
        return {};
    }

    const std::string normalized = to_lower(input);
    const auto special_cases = make_special_cases();
    const auto found = special_cases.find(normalized);
    if (found != special_cases.end()) {
        return found->second;
    }

    std::string result = fallback_transliterate(normalized);
    if (result.empty()) {
        return normalized;
    }
    return result;
}

std::vector<kalaime::layout::Candidate> KalaPhoneticLayout::generate_candidates(
    const std::string& input) const {
    std::vector<kalaime::layout::Candidate> out;
    const std::string primary = transliterate(input);
    out.push_back({primary, 1.0, "primary phonetic output"});

    const auto special_cases = make_special_cases();
    auto iter = special_cases.find(to_lower(input));
    if (iter != special_cases.end()) {
        out.push_back({iter->second + " (dictionary)", 0.9, "known Bengali word"});
    }
    if (primary != input && !primary.empty()) {
        out.push_back({primary, 0.75, "fallback romanization"});
    }
    return out;
}

}  // namespace kalaime::layouts
