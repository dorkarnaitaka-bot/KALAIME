#pragma once

#include <string>
#include <vector>

namespace kalaime::unicode {

struct BengaliCharacterInfo {
    std::string character;
    std::string name;
    std::string category;
    std::string romanization;
    std::string normalized_form;
};

std::string normalize_nfkc(const std::string& text);
std::vector<BengaliCharacterInfo> supported_bengali_inventory();
bool is_bengali_codepoint(unsigned int code_point);
std::vector<std::string> split_graphemes(const std::string& text);

}  // namespace kalaime::unicode
