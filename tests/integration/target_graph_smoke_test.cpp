#include <gtest/gtest.h>
#include <sqlite3.h>
#include <unicode/uversion.h>

#include "kalaime/core/project.hpp"
#include "kalaime/dictionary/dictionary.hpp"
#include "kalaime/layouts/phonetic/kala_phonetic_layout.hpp"
#include "kalaime/unicode/bengali.hpp"

TEST(TargetGraphSmokeTest, LinksPortableCoreAndProductionDependencies) {
    EXPECT_NE(kalaime::project_abi_version(), 0U);
    EXPECT_GT(sqlite3_libversion_number(), 0);

    UVersionInfo icu_version{};
    u_getVersion(icu_version);
    EXPECT_GT(icu_version[0], 0);

    const auto inventory = kalaime::unicode::supported_bengali_inventory();
    EXPECT_FALSE(inventory.empty());
    EXPECT_TRUE(kalaime::unicode::is_bengali_codepoint(0x0985));
}

TEST(TargetGraphSmokeTest, DictionaryAndLayoutManageRealInputWorkflow) {
    constexpr int kAmiFrequency = 3;
    constexpr int kBanglaFrequency = 5;

    const std::string db_path = "/tmp/kalaime_dictionary_test.sqlite";
    kalaime::dictionary::Dictionary dictionary(db_path);
    dictionary.add_entry("আমি", "ami", kAmiFrequency);
    dictionary.add_entry("বাংলা", "bangla", kBanglaFrequency);

    const auto matches = dictionary.find_matches("ami");
    EXPECT_FALSE(matches.empty());
    EXPECT_EQ(matches.front(), "আমি");

    const kalaime::layouts::KalaPhoneticLayout layout;
    EXPECT_EQ(layout.transliterate("ami"), "আমি");
    EXPECT_EQ(layout.transliterate("bangla"), "বাংলা");
}
