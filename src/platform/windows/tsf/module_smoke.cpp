#include "kalaime/platform/windows/tsf/module_smoke.hpp"

#include <Windows.h>
#include <msctf.h>
#include <wil/resource.h>

static_assert(sizeof(ITfThreadMgr*) == sizeof(void*));

unsigned int kalaime_windows_module_smoke() noexcept {
    wil::unique_hmodule module;
    return static_cast<unsigned int>(::GetCurrentProcessId()) + (module.get() == nullptr ? 0U : 1U);
}
