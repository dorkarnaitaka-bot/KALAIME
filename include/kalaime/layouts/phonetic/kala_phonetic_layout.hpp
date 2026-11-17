#pragma once

#include <string>
#include <vector>

#include "kalaime/layout-api/phonetic_layout.hpp"

namespace kalaime::layouts {

class KalaPhoneticLayout final : public kalaime::layout::PhoneticLayout {
   public:
    std::string transliterate(const std::string& input) const override;
    std::vector<kalaime::layout::Candidate> generate_candidates(
        const std::string& input) const override;
};

}  // namespace kalaime::layouts
