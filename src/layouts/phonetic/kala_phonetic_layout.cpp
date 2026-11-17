#include "kalaime/layouts/phonetic/kala_phonetic_layout.hpp"

#include <string>
#include <vector>

#include "phonetic_parser.hpp"

namespace kalaime::layouts {
std::string KalaPhoneticLayout::transliterate(const std::string& input) const {
    return kalaime::layouts::phonetic::detail::transliterate_primary(input);
}

std::vector<kalaime::layout::Candidate> KalaPhoneticLayout::generate_candidates(
    const std::string& input) const {
    return kalaime::layouts::phonetic::detail::generate_candidates(input);
}

}  // namespace kalaime::layouts
