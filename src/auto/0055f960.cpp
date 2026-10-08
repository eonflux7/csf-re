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

// Hex-Rays name: sub_6683F0. Takes the menu object in ecx, returns an object pointer in eax.
CSF_FUNC(void*, __fastcall, MenuMpOnline_GetNetworkModeObject, 0x006683F0, (void* self, void* edx));

// Hex-Rays name: sub_55F790. Its own address is bound here as a callee.
CSF_FUNC(int, __fastcall, MenuMpOnline_TransitionForNetworkMode, 0x0055F790, (void* self, void* edx));

// Dispatches a menu command. a2 == 1 transitions the network mode; a2 == 50 returns a3,
// or, when a3's flag byte at +0x38 is set, calls the virtual method at vtbl slot 7 on the
// object returned by MenuMpOnline_GetNetworkModeObject.
static int __fastcall MenuMpOnline_HandleCommand(void* self, void* /*edx*/, int a2, int a3)
{
    if (a2 == 1)
    {
        // Original: add ecx, -4 (sub-object one std::uint32_t before this).
        void* subObject = reinterpret_cast<uint8_t*>(self) - 4;
        return MenuMpOnline_TransitionForNetworkMode(subObject, nullptr);
    }

    int result = a2 - 50;
    if (a2 == 50)
    {
        result = a3;
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(a3), 0x38))
        {
            void* networkObject = MenuMpOnline_GetNetworkModeObject(self, nullptr);
            return reinterpret_cast<int (__fastcall*)(void*, void*)>(CSF_AT(void**, networkObject, 0)[7])(networkObject, nullptr);
        }
    }
    return result;
}

HOOK(0x0055F960, MenuMpOnline_HandleCommand, "auto");
