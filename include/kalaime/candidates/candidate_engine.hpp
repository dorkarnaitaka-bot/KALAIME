#pragma once

#include <string>
#include <vector>

#include "kalaime/layout-api/phonetic_layout.hpp"

namespace kalaime::candidates {

class CandidateEngine {
   public:
    explicit CandidateEngine(const kalaime::layout::PhoneticLayout& layout);

    std::vector<kalaime::layout::Candidate> make_candidates(const std::string& input) const;

   private:
    const kalaime::layout::PhoneticLayout& layout_;
};

}  // namespace kalaime::candidates
