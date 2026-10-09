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

// Grows the element array when full (sub_44F8E0 is the original's grow routine).
CSF_FUNC(int, __fastcall, sub_44F8E0, 0x0044F8E0, (void* self, void* edx, int a2));

// Appends one 16-byte element (four dwords copied from source) to the array and
// returns a pointer to the newly written slot. Grows the array first when the
// count has reached the capacity.
static uint32_t* __fastcall S_0044f8e0_AppendElement16(S_0044f8e0* self, void* /*edx*/, uint32_t* source)
{
    if (self->field_0008 >= self->field_0004)
    {
        sub_44F8E0(self, nullptr, static_cast<int>(self->field_000c));
    }

    // Count is re-read after a possible grow.
    uint32_t slotIndex = self->field_0008;
    self->field_0008 = slotIndex + 1;

    uint32_t* slot = reinterpret_cast<uint32_t*>(self->field_0000 + 16u * slotIndex);
    slot[0] = source[0];
    slot[1] = source[1];
    slot[2] = source[2];
    slot[3] = source[3];
    return slot;
}

HOOK(0x00450A80, S_0044f8e0_AppendElement16, "auto");
