#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// int __cdecl GetGlobalState(void)
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x50D190, ());

// Reads a flag byte and a float threshold from the menu page object, then
// checks whether the current menu-state value reaches that threshold.
// Layout: byte +0 = cached result, byte +1 = enabled flag, float +4 = threshold.
static int __fastcall MenuPage_IsReadyAndAboveThreshold(float* self, void* /*edx*/)
{
    void* globalState = GetGlobalState();
    void* menuObj = CSF_AT(void*, globalState, 0x14);

    int result = 0;
    if (CSF_AT(uint8_t, self, 1) != 0)
    {
        // vtbl slot +0xA0: returns byte flag
        uint8_t (__fastcall* isActive)(void*, void*) =
            reinterpret_cast<uint8_t (__fastcall*)(void*, void*)>(CSF_AT(void**, menuObj, 0)[0xA0 / 4]);
        result = isActive(menuObj, nullptr) != 0 ? 1 : 0;
    }

    *reinterpret_cast<uint8_t*>(self) = static_cast<uint8_t>(result);

    if (result == 0)
        return 0;

    // vtbl slot +0xA4: returns a value compared against the threshold
    double (__fastcall* getLevel)(void*, void*) =
        reinterpret_cast<double (__fastcall*)(void*, void*)>(CSF_AT(void**, menuObj, 0)[0xA4 / 4]);
    double level = getLevel(menuObj, nullptr);

    double threshold = static_cast<double>(CSF_AT(float, self, 4));
    // NaN compares false, matching the original (fcomp + test ah,1 path).
    return (level >= threshold) ? 1 : 0;
}

HOOK(0x0050C880, MenuPage_IsReadyAndAboveThreshold, "auto");
