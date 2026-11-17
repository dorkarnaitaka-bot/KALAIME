#include "kalaime/candidates/candidate_engine.hpp"

namespace kalaime::candidates {

CandidateEngine::CandidateEngine(const kalaime::layout::PhoneticLayout& layout) : layout_(layout) {}

std::vector<kalaime::layout::Candidate> CandidateEngine::make_candidates(
    const std::string& input) const {
    return layout_.generate_candidates(input);
}

}  // namespace kalaime::candidates
