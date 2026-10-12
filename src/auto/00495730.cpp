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

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));

// Returns true when the context action is available: either the sub-object
// reached through field_0044 reports state 27, or the signed flag byte at +0x2E0 is set.
static bool __fastcall C_008342e8_IsContextActionAvailable(C_008342e8* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0044));
    C_008319c0* v2 = CSF_AT(C_008319c0*, owner, 0x9C);

    if (v2 && C_008319c0_GetSubObjectField74(v2, nullptr) == 27)
        return true;

    return CSF_AT(int8_t, self, 0x2E0) < 0;
}

HOOK(0x495730, C_008342e8_IsContextActionAvailable, "auto");
