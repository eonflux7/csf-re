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

// Releases the object reference held by an element (ecx = element+0xB8 pointer + 4).
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));

// Releases the object reference at +0xB8 of every 196-byte element in the
// array owned by this object, clearing each slot after release.
static int __fastcall C_00831008_ReleaseElementObjects(C_00831008* self, void* /*edx*/)
{
    int elementCount = 0;

    if (self->field_0034)
    {
        int32_t byteSpan = static_cast<int32_t>(self->field_0038 - self->field_0034);
        elementCount = byteSpan / 196;
    }

    if (elementCount)
    {
        uint32_t byteOffset = 0;
        int remaining = elementCount;
        do
        {
            uint32_t objRef = CSF_AT(uint32_t,
                reinterpret_cast<uint8_t*>(self->field_0034), byteOffset + 0xB8);
            if (objRef)
            {
                ReleaseObjectReference(reinterpret_cast<void*>(objRef + 4), nullptr);
                CSF_AT(uint32_t,
                    reinterpret_cast<uint8_t*>(self->field_0034), byteOffset + 0xB8) = 0;
            }
            byteOffset += 196;
            --remaining;
        } while (remaining != 0);
    }

    return 0;
}

HOOK(0x43E6C0, C_00831008_ReleaseElementObjects, "auto");
