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

// Notifies the audio backend object (stored at +0x30 of the child) with mode 2/1 and the new value.
CSF_FUNC(uint32_t*, __cdecl, sub_7E1380, 0x007E1380, (uint32_t* a1, int a2, int a3, ...));

// C_00863dd0 (vtable 0x00863DD0) method. Stores a2 into the child's +0x70 slot, then, if the
// child's +0x30 pointer is set, forwards that pointer and the value to sub_7E1380.
// Original: ret 4 (one stack argument, __thiscall).
static int __fastcall C_00863dd0_SetEffectValueAndNotify(C_00863dd0* self, void* /*edx*/, int a2)
{
    CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0004), 0x70) = a2;

    uint32_t result = self->field_0004;
    uint32_t backend = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004), 0x30);
    if (backend != 0)
    {
        return static_cast<int>(reinterpret_cast<uintptr_t>(
            sub_7E1380(reinterpret_cast<uint32_t*>(backend), 2, 1, a2)));
    }
    return static_cast<int>(result);
}

HOOK(0x00712680, C_00863dd0_SetEffectValueAndNotify, "auto");
