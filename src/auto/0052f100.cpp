#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bound original callees
CSF_FUNC(uint8_t, __cdecl, MenuScenarioSelect_QueryFlag, 0x007A8ED0, (int a1));
CSF_FUNC(int, __cdecl, MenuScenarioSelect_FallbackHandle, 0x00428500, (int a1));

// Original: int __thiscall sub_52F100(this, a2, a3); ret 8
// If the query on a2 is true, dispatch virtually (slot 0x4C) on the object
// located 4 bytes before `this` (the embedded base at this-4); otherwise
// forward a2 to the fallback handler.
static int __fastcall MenuScenarioSelect_DispatchOnFlag(void* self, void* /*edx*/, int a2, int /*a3*/)
{
    if (MenuScenarioSelect_QueryFlag(a2))
    {
        void* baseObj = reinterpret_cast<uint8_t*>(self) - 4;
        void** vtbl = CSF_AT(void**, baseObj, 0);
        return ((int(__fastcall*)(void*, void*))vtbl[0x4C / 4])(baseObj, nullptr);
    }
    return MenuScenarioSelect_FallbackHandle(a2);
}

HOOK(0x0052F100, MenuScenarioSelect_DispatchOnFlag, "auto");
