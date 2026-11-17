#include <gtest/gtest.h>

#include "kalaime/platform/windows/tsf/module_smoke.hpp"

TEST(WindowsBuildSmokeTest, LoadsTheNativeWindowsModuleTarget) {
    EXPECT_NE(kalaime_windows_module_smoke(), 0U);
}
