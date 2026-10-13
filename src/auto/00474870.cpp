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

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

// Allocates a 52-byte C_0082d478-sized copy, resets its fields to defaults,
// then copies the state of `self` into it. Returns the new object (or 0 on
// allocation failure, in which case the copy writes go through a null pointer,
// as the original does).
static int __fastcall C_0082d478_CreateCopy(C_0082d478* self, void* /*edx*/)
{
    uint8_t* copy = reinterpret_cast<uint8_t*>(CoreMemory_AllocUntagged(52));
    if (copy)
    {
        C_0082d478* fresh = reinterpret_cast<C_0082d478*>(copy);
        CSF_AT(void*, copy, 0x0) = CSF_VTBL(0x0082D478);
        CSF_AT(void*, copy, 0x8) = CSF_VTBL(0x0082D448);
        fresh->field_000c = 0xFFFFFFFFu;
        fresh->field_0010 = 0xFFFFFFFFu;
        CSF_AT(void*, copy, 0x14) = CSF_VTBL(0x0082D448);
        fresh->field_0018 = 0xFFFFFFFFu;
        fresh->field_001c = 0xFFFFFFFFu;
        fresh->field_0004 = 0xFFFFFFFFu;
        fresh->field_0020 = 0;
        fresh->field_0024 = 0;
        fresh->field_002c = 0;
        fresh->field_0028 = 0;
        fresh->field_0030 = 0;
    }
    else
    {
        copy = nullptr;
    }

    C_0082d478* dst = reinterpret_cast<C_0082d478*>(copy);
    dst->field_0004 = self->field_0004;
    dst->field_000c = self->field_000c;
    dst->field_0010 = self->field_0010;
    dst->field_0018 = self->field_0018;
    dst->field_001c = self->field_001c;
    dst->field_0024 = self->field_0024;
    dst->field_002c = self->field_002c;
    dst->field_0020 = self->field_0020;
    dst->field_0030 = self->field_0030;
    dst->field_0028 = self->field_0028;

    return reinterpret_cast<int>(copy);
}

HOOK(0x00474870, C_0082d478_CreateCopy, "auto");
