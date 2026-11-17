#pragma once

#include <string>
#include <vector>

namespace kalaime::layout {

struct Candidate {
    std::string text;
    double score = 0.0;
    std::string rationale;
};

class PhoneticLayout {
   public:
    virtual ~PhoneticLayout() = default;
    virtual std::string transliterate(const std::string& input) const = 0;
    virtual std::vector<Candidate> generate_candidates(const std::string& input) const = 0;
};

}  // namespace kalaime::layout
