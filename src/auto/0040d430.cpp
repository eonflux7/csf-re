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

// Callee: unnamed in the packet; bound by address with the packet's signature.
CSF_FUNC(unsigned int, __fastcall, C_0082dc88_RefreshState, 0x0040D380, (void* self, void* edx));

// Copies a 4-float vector (raw dwords, as the original does) into the object's
// stored vector at +0x20, marks the object dirty, advances the running
// coordinate at +0x48 and the secondary coordinate at +0x44 by a2[2], then
// refreshes the state. Does nothing if bit 1 of the flags at +0x34 is set.
static unsigned int __fastcall C_0082dc88_StoreVectorAndAdvance(C_0082dc88* self, void* /*edx*/, float* a2)
{
    unsigned int result = 0;

    if ((self->field_0034 & 2) == 0)
    {
        float runningPosition = self->field_0048;

        uint32_t* source = reinterpret_cast<uint32_t*>(a2);
        self->field_0020 = source[0];
        self->field_0024 = source[1];
        self->field_0028 = source[2];
        self->field_002c = source[3];

        self->field_0039 = 1;

        self->field_0048 = runningPosition + a2[2];
        self->field_0044 = self->field_0044 + a2[2];

        result = C_0082dc88_RefreshState(self, nullptr);
    }

    return result;
}

HOOK(0x0040D430, C_0082dc88_StoreVectorAndAdvance, "auto");
