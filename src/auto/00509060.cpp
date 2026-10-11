#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, AppMain_GetRequestedOption, 0x00508FF0, ());
CSF_FUNC(char, __cdecl, AppMain_PrepareConfiguredOption, 0x0069C660, (int a1));
CSF_FUNC(int, __cdecl, AppMain_ApplyOption, 0x0069C760, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

static int __cdecl AppMain_ApplySelectedOption()
{
    const int requestedOption = AppMain_GetRequestedOption();
    void* configuration = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(GetGlobalValue_75FDF0()));
    const int configuredOption = CSF_AT(int, configuration, 0x38);

    AppMain_PrepareConfiguredOption(configuredOption);

    switch (requestedOption)
    {
    case 0:  return AppMain_ApplyOption(0);
    case 1:  return AppMain_ApplyOption(1);
    case 2:  return AppMain_ApplyOption(2);
    case 3:  return AppMain_ApplyOption(3);
    case 4:  return AppMain_ApplyOption(4);
    case 5:  return AppMain_ApplyOption(5);
    case 6:  return AppMain_ApplyOption(6);
    case 7:  return AppMain_ApplyOption(7);
    case 8:  return AppMain_ApplyOption(8);
    case 9:  return AppMain_ApplyOption(9);
    case 10: return AppMain_ApplyOption(10);
    case 11: return AppMain_ApplyOption(11);
    case 12: return AppMain_ApplyOption(12);
    case 13: return AppMain_ApplyOption(13);
    case 14: return AppMain_ApplyOption(14);
    default: return AppMain_ApplyOption(configuredOption);
    }
}

HOOK(0x00509060, AppMain_ApplySelectedOption, "auto");
