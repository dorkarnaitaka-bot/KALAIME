#include <gtest/gtest.h>

#include "kalaime/core/project.hpp"
#include "kalaime/layouts/phonetic/kala_phonetic_layout.hpp"

TEST(ProjectSmokeTest, ExposesInitialInfrastructureAbiVersion) {
    EXPECT_EQ(kalaime::project_abi_version(), 1U);
}

TEST(ProjectSmokeTest, HandlesCoreBanglaTransliterationExamples) {
    const kalaime::layouts::KalaPhoneticLayout layout;

    EXPECT_EQ(layout.transliterate("ami"), "আমি");
    EXPECT_EQ(layout.transliterate("bangla"), "বাংলা");
    EXPECT_EQ(layout.transliterate("bangladesh"), "বাংলাদেশ");
    EXPECT_EQ(layout.transliterate("bhasha"), "ভাষা");
    EXPECT_EQ(layout.transliterate("shikkha"), "শিক্ষা");
    EXPECT_EQ(layout.transliterate("bhalobasha"), "ভালোবাসা");
}
