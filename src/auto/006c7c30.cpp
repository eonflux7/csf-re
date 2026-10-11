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

// Callee: releases the object held by a hud entry (sub_6C47F0, __thiscall, ret 0).
CSF_FUNC(int, __fastcall, ReleaseHudEntryObject, 0x006C47F0, (void* self, void* /*edx*/));

// Callee: sets the entry's field 0x0C on the object (__thiscall, ret 4).
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* /*edx*/, int value));

// Walks the hud entry array from the last entry backwards (12-byte entries: object, value, flag).
// Each entry is released, then its object gets SetField0C, and the count is decremented.
// Loops while the entry's flag byte is nonzero; returns the last flag byte read.
static char __fastcall GlobalHud_Commit_6C7C30(C_0085e7b8* self, void* /*edx*/)
{
    char flag = 0;
    do
    {
        // Base array at +0x14, count at +0x1C; both are re-read every iteration.
        uintptr_t entryBase = static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x14));
        uint32_t count = CSF_AT(uint32_t, self, 0x1c);
        uint32_t* entry = reinterpret_cast<uint32_t*>(entryBase + count * 12u - 12u);

        C_008670f0* obj = reinterpret_cast<C_008670f0*>(static_cast<uintptr_t>(entry[0]));
        int value = static_cast<int>(entry[1]);
        flag = static_cast<char>(entry[2]);

        ReleaseHudEntryObject(obj, nullptr);
        C_008670f0_SetField0C(obj, nullptr, value);

        // Count is reloaded after the calls, as in the original.
        CSF_AT(uint32_t, self, 0x1c) = CSF_AT(uint32_t, self, 0x1c) - 1;
    } while (flag != 0);

    return flag;
}

HOOK(0x006C7C30, GlobalHud_Commit_6C7C30, "auto");
