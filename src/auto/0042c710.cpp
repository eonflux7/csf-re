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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Restores the C_00830578 vtable chain down to CMenuScreen, releasing the
// owned block at +0xE4 and, when bit 0 of a2 is set, the object itself.
// Returns the object pointer (the original returns &this->__vftable).
static uint32_t* __fastcall C_00830578_Destroy(C_00830578* self, void* /*edx*/, char a2)
{
    *(void***)self = CSF_VTBL(0x00830578);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00830528));
    CSF_AT(uint32_t, self, 0x10) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00830520));

    if (self->field_00e4) {
        Mem_Free(reinterpret_cast<void*>(self->field_00e4));
        self->field_00e4 = 0;
    }

    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00830488));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00830438));
    *(void***)self = CSF_VTBL(0x0083042C);

    if ((a2 & 1) != 0) {
        Mem_Free(reinterpret_cast<void*>(self));
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0042C710, C_00830578_Destroy, "auto");
