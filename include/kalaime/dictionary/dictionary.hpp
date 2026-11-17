#pragma once

#include <string>
#include <vector>

namespace kalaime::dictionary {

class Dictionary {
   public:
    explicit Dictionary(const std::string& database_path);
    ~Dictionary();

    void add_entry(const std::string& word, const std::string& normalized, int frequency = 1);
    std::vector<std::string> find_matches(const std::string& normalized) const;
    std::vector<std::string> words_for_prefix(const std::string& prefix) const;
    void initialize_schema();

   private:
    struct Impl;
    Impl* impl_;
};

}  // namespace kalaime::dictionary
