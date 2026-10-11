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

CSF_FUNC(int, __cdecl, GetValue8BBB28, 0x006F7820, ());
CSF_FUNC(char, __fastcall, HAnim_ReleaseTrackHandle, 0x006F7A70, (void* self, void* edx, void** a2));

// Clears the two track slots at +4 (released through HAnim_ReleaseTrackHandle when set)
// and zeroes the fields at +8 and +0C. Plain stdcall function: never reads ecx.
static int __stdcall HAnim_ClearTrackSlots(uint32_t* a1)
{
    int result = static_cast<int>(a1[1]);
    if (result != 0)
    {
        void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8BBB28()));
        result = static_cast<int>(HAnim_ReleaseTrackHandle(owner, nullptr, reinterpret_cast<void**>(a1 + 1)));
        a1[1] = 0;
    }
    a1[2] = 0;
    a1[3] = 0;
    return result;
}

HOOK(0x6E8DF0, HAnim_ClearTrackSlots, "auto");
