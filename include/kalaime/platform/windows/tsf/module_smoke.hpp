#pragma once

#if defined(KALAIME_WINDOWS_TSF_EXPORTS)
#define KALAIME_WINDOWS_TSF_API __declspec(dllexport)
#else
#define KALAIME_WINDOWS_TSF_API __declspec(dllimport)
#endif

extern "C" KALAIME_WINDOWS_TSF_API unsigned int kalaime_windows_module_smoke() noexcept;
